// Fill out your copyright notice in the Description page of Project Settings.


#include "MPlayerCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Engine/World.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/KismetMathLibrary.h"
#include "InputActionValue.h"
#include "DrawDebugHelpers.h"
#include "../InventorySystem/InventorySystem.h"
#include "AbilitySystemComponent.h"
#include "../AbilitySystem/Attributes/MAttributeSet.h"
#include "../AbilitySystem/MAbilitySystemComponent.h"
#include "../AbilitySystem/Abilities/MGameplayAbility.h"
#include "MTeamManager.h"
#include "Blueprint/UserWidget.h"
#include "../Renders/Widgets/PlayerHUDWidget.h"
#include "../Enemy/MEnemy_Base.h"
#include "MPlayerControllerBase.h"
#include "GameplayEffectTypes.h"
#include "GameplayTagContainer.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "kismet/GameplayStatics.h"



// Sets default values

AMPlayerCharacter::AMPlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	// Create Ability System Component
	AbilitySystemComponent = CreateDefaultSubobject<UMAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	//AttributeSet = CreateDefaultSubobject<UMAttributeSet>(TEXT("Attributes"));

	InventorySystem = CreateDefaultSubobject<UInventorySystem>(TEXT("InventorySystem"));
	InventorySystem->Capacity = 20;

	DeathTag = FGameplayTag::RequestGameplayTag(FName("State.Dead"));
	EffectRemoveOnDeathTag = FGameplayTag::RequestGameplayTag(FName("Effect.RemoveOnDeath"));

	

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UMAttributeSet::GetHealthAttribute()).AddUObject(this, &AMPlayerCharacter::OnHealthChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UMAttributeSet::GetMaxHealthAttribute()).AddUObject(this, &AMPlayerCharacter::OnMaxHealthChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UMAttributeSet::GetEnergyAttribute()).AddUObject(this, &AMPlayerCharacter::OnEnergyChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UMAttributeSet::GetMaxEnergyAttribute()).AddUObject(this, &AMPlayerCharacter::OnMaxEnergyChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UMAttributeSet::GetArmorAttribute()).AddUObject(this, &AMPlayerCharacter::OnArmorChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UMAttributeSet::GetMaxArmorAttribute()).AddUObject(this, &AMPlayerCharacter::OnMaxArmorChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UMAttributeSet::GetLevelAttribute()).AddUObject(this, &AMPlayerCharacter::OnLevelChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UMAttributeSet::GetExperienceAttribute()).AddUObject(this, &AMPlayerCharacter::OnExperienceChanged);

	
	EnemyDetectionCollider = CreateDefaultSubobject<USphereComponent>(TEXT("Enemy Detection Collider"));
	EnemyDetectionCollider->SetupAttachment(RootComponent);
	EnemyDetectionCollider->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	EnemyDetectionCollider->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn,
		ECollisionResponse::ECR_Overlap);
	EnemyDetectionCollider->SetSphereRadius(TargetLockDistance);

	Weapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(GetMesh(), "RightHandItem");
	Weapon->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

	PassiveMovementSpeed = 450.0f;
	CombatMovementSpeed = 250.0f;
	GetCharacterMovement()->MaxWalkSpeed = PassiveMovementSpeed; 

	//PlayerHUDComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("PlayerHUDComponent"));
	//PlayerHUDComponent->SetupAttachment(RootComponent);
	//PlayerHUDComponent->SetRelativeLocation(FVector(0.0f, -50.0f, 120.0f));
	//PlayerHUDComponent->SetWidgetSpace(EWidgetSpace::Screen);
	//PlayerHUDComponent->SetDrawSize(FVector2D(25.0f, 25.0f));

	PlayerHUDWidgetClass = StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("/Game/MachinaAutomadum/Renders/Widgets/W_PlayerHUDWidget.W_PlayerHUDWidget_CT"));
	if (!PlayerHUDWidgetClass)
	{
		UE_LOG(LogTemp, Error, TEXT("%s() Failed to find PlauerHUD. If it was moved, please update the reference location in C++."), *FString(__FUNCTION__));
	}

	TeamManager = CreateDefaultSubobject<UMTeamManager>(TEXT("TeamManager"));
	if(TeamManager) TeamManager->CurrentCharacter = this;

	//Bind Delegates for Attributes
	

	
}	

// Called when the game starts or when spawned
void AMPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	InitAbilitySystem();
	if(IsValid(AbilitySystemComponent))
	{
		AttributeSet = AbilitySystemComponent->GetSet<UMAttributeSet>();
		if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Attribute Set is valid"));
	}
	else{
		if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Attribute Set is not valid"));
	}

	/* Setting up Widget HUD*/

	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PC && PC->IsLocalPlayerController())
		{
		if(PlayerHUDWidgetClass)
		{
			
		 	PlayerHUD = CreateWidget<UPlayerHUDWidget>(PC, PlayerHUDWidgetClass);
			PlayerHUD->AddToViewport();

			if(PlayerHUD && PlayerHUDComponent)
			{
				if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("BindingToHUD"));
				//PlayerHUDComponent->SetWidget(PlayerHUD);
				//PlayerHUD = Cast<UPlayerHUDWidget>(HudWidget);
				

				PlayerHUD->SetHealth(AttributeSet->GetHealth(), AttributeSet->GetMaxHealth());
				PlayerHUD->SetEnergy(AttributeSet->GetEnergy(), AttributeSet->GetMaxEnergy());
				PlayerHUD->SetArmor(AttributeSet->GetArmor(), AttributeSet->GetMaxArmor());
				PlayerHUD->SetLevel(AttributeSet->GetLevel());
				PlayerHUD->SetExperience(AttributeSet->GetExperience(), 500.0f);
		}
		else{
			if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("PlayerHUDNotFound"));
		}
	}
	}

		EnemyDetectionCollider->OnComponentBeginOverlap.AddDynamic(this, &AMPlayerCharacter::OnEnemyDetectionBeginOverlap);
		EnemyDetectionCollider->OnComponentEndOverlap.AddDynamic(this, &AMPlayerCharacter::OnEnemyDetectionEndOverlap);


		TSet<AActor*> NearActors;
		EnemyDetectionCollider->GetOverlappingActors(NearActors);
		for(auto& EnemyActor : NearActors)
		{
			AMEnemy_Base* EnemyCharacter = Cast<AMEnemy_Base>(EnemyActor);
			if(EnemyCharacter)
			{
				NearbyEnemies.Add(EnemyCharacter);
			}
		}
	
}

void AMPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(CombatMCMappingContext, 0);
		}
	}
	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AMPlayerCharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AMPlayerCharacter::StopJumping);
       
	   EnhancedInputComponent->BindAction(ClimbAction, ETriggerEvent::Started, this, &AMPlayerCharacter::StartClimb);
	   EnhancedInputComponent->BindAction(ClimbAction, ETriggerEvent::Completed, this, &AMPlayerCharacter::StopClimb);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMPlayerCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMPlayerCharacter::Look);

		// Dash
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Triggered, this, &AMPlayerCharacter::Dash);


		BindInputToAbilities(EnhancedInputComponent);
	}
	else
	{
		
	}

	
}

void AMPlayerCharacter::SwitchToCombatContext()
{
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))

	{
		 if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
        {
            // Replace the roaming mapping context with the combat mapping context
             Subsystem->RemoveMappingContext(RoamingMCMappingContext);
            Subsystem->AddMappingContext(CombatMCMappingContext, 0);
        }
	}
}

void AMPlayerCharacter::SwitchToRoamingContext()
{
    if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
        {
            // Replace the combat mapping context with the roaming mapping context
         	Subsystem->RemoveMappingContext(CombatMCMappingContext);
            Subsystem->AddMappingContext(RoamingMCMappingContext, 0);
        }
    }
}

void AMPlayerCharacter::OpenCloseMenu()
{

}

void AMPlayerCharacter::Interact(const FInputActionValue &Value)
{

}

void AMPlayerCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		if(!bIsClimbing)
		{
			AddMovementInput(ForwardDirection, MovementVector.Y);
			AddMovementInput(RightDirection, MovementVector.X);
		}
		else{
			//Climbing
			FVector UpVector = GetActorUpVector();
			const FVector UpDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Z);
			UpVector.Z = UpDirection.Z;
			const FVector SlideDirection = GetActorRightVector();
			AddMovementInput(UpDirection, MovementVector.Y);
			AddMovementInput(SlideDirection, MovementVector.X);
		}

		InputDirection = FVector(MovementVector.Y, MovementVector.X, 0.0f);
		
	}
}

void AMPlayerCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AMPlayerCharacter::Sprint()
{
	if(!bIsSprinting)
	{
		bIsSprinting = true;
		GetCharacterMovement()->MaxWalkSpeed = 1000.f;
		GetWorldTimerManager().SetTimer(SprintTimerHandle, this, &AMPlayerCharacter::StopSprinting, 2.0f, false);
	}
}

void AMPlayerCharacter::StopSprinting()
{
	bIsSprinting = false;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
}

void AMPlayerCharacter::Dash()
{
	//if(!bIsDashing)
	//{
	
	//	bIsDashing = true;
	//	//Calculate the dash offset
		//FVector DashOffset = GetActorForwardVector() * DashDistance;
//
	//	AddActorWorldOffset(DashOffset, true, nullptr, ETeleportType::TeleportPhysics);

	//	GetWorldTimerManager().SetTimer(DashTimerHandle, this, &AMPlayerCharacter::StopDashing, 1.0f, false);
		
	//}
}

void AMPlayerCharacter::StopDashing()
{
	//bIsDashing = false;
	//GetCharacterMovement()->MaxWalkSpeed = 500.f;
}

void AMPlayerCharacter::Jump()
{
	//if (GetCharacterMovement()->IsFalling() == false || JumpCount < MaxJumpCount)
	//{
		
		//Super::Jump();
		 // Calculate the jump velocity

		//FVector Velocity = GetVelocity();
		//Velocity.Z = 0;
		//GetCharacterMovement()->Velocity = Velocity;

        //FVector JumpVelocity = FVector(0, 0, GetCharacterMovement()->JumpZVelocity);
        
        // Launch the character into the air
       // LaunchCharacter(JumpVelocity, false, false);

		//JumpCount++;
	//}
}

//keep
void AMPlayerCharacter::StopJumping() //double jump bug please fix
{
	Super::StopJumping();

}

void AMPlayerCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	JumpCount = 0;
}


void AMPlayerCharacter::StartClimb()
{
	FHitResult Hit;

    bool bHit = WallLineTrace(Hit);
	if (!bIsClimbing && bHit )
	{
		
	   
		bIsClimbing = true;
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
		GetCharacterMovement()->bOrientRotationToMovement = false;
        FRotator NormalDirection = Hit.Normal.Rotation();
		NormalDirection.Yaw += 180.0f;

		FRotator CurrentRotation = GetActorRotation();
		CurrentRotation.Yaw = NormalDirection.Yaw;

		SetActorRotation(NormalDirection);
		

	}
	else if(!bHit){
		StopClimb();
	}

}

void AMPlayerCharacter::StopClimb()
{
	
		bIsClimbing = false;
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Falling);
		GetCharacterMovement()->bOrientRotationToMovement = true;
	
}



bool AMPlayerCharacter::LineTrace(FHitResult& Hit, float Distance)
{
	FVector Start = GetActorLocation();
	FVector End = Start + GetActorForwardVector() * Distance;

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, CollisionParams);
	
	FColor TraceColor = bHit ? FColor::Green : FColor::Red;

	if(GetWorld() == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("World is null"));
	}

	DrawDebugLine(GetWorld(), Start, End, TraceColor, true, 1000.f, 5.0f);

	return bHit;
}

bool AMPlayerCharacter::WallLineTrace(FHitResult& Hit)
{
	return LineTrace(Hit, WallLineTraceDistance);

}

void AMPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    
	FHitResult Hit;
    
	FocusTarget();

	//Updating climbing rotation to face the wall
	if (bIsClimbing && WallLineTrace(Hit))
	{
		FRotator NormalDirection = Hit.Normal.Rotation();
		NormalDirection.Yaw += 180.0f;

		FRotator CurrentRotation = GetActorRotation();
		CurrentRotation.Yaw = NormalDirection.Yaw;

		SetActorRotation(NormalDirection);
	}
	else if(bIsClimbing){
		StopClimb();
	}
	
}

UAbilitySystemComponent* AMPlayerCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AMPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	//Server GAS init
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);


	}

	//InitializeAttributes();
	//GiveDefaultAbilities();

}

void AMPlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	}

	//InitializeAttributes();
}

//void AMPlayerCharacter::InitializeAttributes()
//{
////	if (AbilitySystemComponent && DefaultAttributeEffect) 
	//{
	//	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	//	EffectContext.AddSourceObject(this);

	//	FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributeEffect, GetCharacterLevel(), EffectContext);
		
	//	if (NewHandle.IsValid())
	//	{
		//	FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*NewHandle.Data.Get());
		//}
	//}
	//else{
		//Bif(GEngine)
		//{
		//	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Unable to initialize attributes"));
		//	AttributeSet = AbilitySystemComponent->GetSet<UMAttributeSet>();
		
		//}
	//}
//}

void AMPlayerCharacter::GiveDefaultAbilities()
{
	if (HasAuthority() && AbilitySystemComponent)
	{
		for (TSubclassOf<UMGameplayAbility> & StartupAbility : DefaultAbilities)
		{
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(StartupAbility.GetDefaultObject(), 1, 0));
		}
	}
}

void AMPlayerCharacter::InitAbilitySystem()
{
	if (PlayerGameplayAbilitiesDataAsset)
	{
		const TSet<FGameplayInputAbilityInfo>& InputAbilities = PlayerGameplayAbilitiesDataAsset->GetInputAbilities();
	
		constexpr int32 AbilityLevel = 1;

		for (const auto& It : InputAbilities)
		{
			if (It.IsValid())
			{
				const FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(It.GameplayAbilityClass, AbilityLevel, It.InputID);
    			AbilitySystemComponent->GiveAbility(AbilitySpec);
				CharacterAbilities.Add(AbilitySpec.Ability->GetClass());
			}
		}
	}

}

void AMPlayerCharacter::OnAbilitySystemInputPressed(const FInputActionValue& Value, int32 CurrentInputID)
{
	
	FString debugMessage = FString::Printf(TEXT("Ability System Input Pressed: %f"), CurrentInputID);
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, debugMessage );
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->AbilityLocalInputPressed(CurrentInputID);
	}
}

void AMPlayerCharacter::OnAbilitySystemInputReleased(const FInputActionValue& Value, int32 CurrentInputID)
{
	
	FString debugMessage = FString::Printf(TEXT("Ability System Input Released: %f"), CurrentInputID);
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, debugMessage );
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->AbilityLocalInputReleased(CurrentInputID);
	}
}

void AMPlayerCharacter::BindInputToAbilities(UEnhancedInputComponent *EnhancedInputComponent)
{
	if (PlayerGameplayAbilitiesDataAsset)
	{
		const TSet<FGameplayInputAbilityInfo>& InputAbilities = PlayerGameplayAbilitiesDataAsset->GetInputAbilities();
		if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, *FString::Printf(TEXT("Input Abilities: %d"), InputAbilities.Num()));
		for (const auto& It : InputAbilities)
		{
			const UInputAction* InputAction = It.InputAction;
			const int32 CurrentInputID = It.InputID;
				
				
			   EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Started, this, &AMPlayerCharacter::OnAbilitySystemInputPressed, CurrentInputID);
     		   EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Completed, this, &AMPlayerCharacter::OnAbilitySystemInputReleased, CurrentInputID);
		}
		

	}
}

int32 AMPlayerCharacter::GetAbilityLevel(int32 AbilityInputID) const
{
    return 1;
}

void AMPlayerCharacter::RemoveCharacterAbilities()
{
	if (GetLocalRole() != ROLE_Authority || AbilitySystemComponent!=nullptr || !AbilitySystemComponent->bCharacterAbilitiesGiven)
	{
		return;
	}

	TArray<FGameplayAbilitySpecHandle> AbilitiesToRemove;
	for (const FGameplayAbilitySpec& Spec : AbilitySystemComponent->GetActivatableAbilities())
	{
		if((Spec.SourceObject == this) && CharacterAbilities.Contains(Spec.Ability->GetClass()))
		{
			AbilitiesToRemove.Add(Spec.Handle); //add to character abilities and remove them 
		}
	}

	for (int32 i = 0; i < AbilitiesToRemove.Num(); i++)
	{
		AbilitySystemComponent->ClearAbility(AbilitiesToRemove[i]);
	}

	AbilitySystemComponent->bCharacterAbilitiesGiven = false;
	

}

void AMPlayerCharacter::AddStartupEffects()
{
	if (GetLocalRole() != ROLE_Authority || AbilitySystemComponent == nullptr)
	{
		return;
	}

	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);
	for (const TSubclassOf<UGameplayEffect>& StartupEffect : StartupEffects)
	{

		FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(StartupEffect, GetCharacterLevel(), EffectContext);
		if (NewHandle.IsValid())
		{
			FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*NewHandle.Data.Get());
		}
	}

	AbilitySystemComponent->bStartupEffectsApplied = true;
}

void AMPlayerCharacter::Die()
{
	FString debugMessage = FString::Printf(TEXT("Player Died"));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, debugMessage );

	RemoveCharacterAbilities();

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->GravityScale = 0.0f;
	GetCharacterMovement()->Velocity = FVector(0.0f, 0.0f, 0.0f);

	OnCharacterDeath.Broadcast(this);

	if(AbilitySystemComponent != nullptr)
	{
		AbilitySystemComponent->CancelAbilities();
		FGameplayTagContainer EffectsTagsToRemove;
		EffectsTagsToRemove.AddTag(EffectRemoveOnDeathTag);
		int32 NumEffectsRemoved = AbilitySystemComponent->RemoveActiveEffectsWithTags(EffectsTagsToRemove);
		AbilitySystemComponent->AddLooseGameplayTag(DeathTag);
	}

	if(DeathMontage){
		PlayAnimMontage(DeathMontage);
	}
	else{
		FinishDying();
	}
}

void AMPlayerCharacter::FinishDying()
{

	FString debugMessage = FString::Printf(TEXT("RemovingCharacter"));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, debugMessage );
	Destroy();
}

void AMPlayerCharacter::UseItem(class UItem* Item)
{
	if(Item)

	{
		Item->UseItem(this);
		InventorySystem->RemoveItem(Item, 1);
	}
}

void AMPlayerCharacter::CycleTarget(bool Clockwise)
{
	AActor* SuitableTarget = nullptr;

	if(Target)
	{
		FVector CameraLocation = 
		Cast<AMPlayerControllerBase>(GetController())->PlayerCameraManager->GetCameraLocation();

		FRotator TargetDirection =
		(Target->GetActorLocation() - CameraLocation).ToOrientationRotator();

		float BestYawDifference = INFINITY;

		for(auto& NearbyEnemy : NearbyEnemies)
		{
			if(NearbyEnemy == Target)
			continue;

			FVector NearbyEnemyDirection = NearbyEnemy->GetActorLocation() - CameraLocation;
			FRotator Difference = UKismetMathLibrary::NormalizedDeltaRotator(NearbyEnemyDirection.ToOrientationRotator(), TargetDirection);
			(NearbyEnemyDirection.ToOrientationRotator(), TargetDirection);

			if(Clockwise && Difference.Yaw <= 0.0f || !(Clockwise && Difference.Yaw >= 0.0f))continue;
			
			float YawDifference = FMath::Abs(Difference.Yaw);
			if(YawDifference < BestYawDifference)
			{
				BestYawDifference = YawDifference;
				SuitableTarget = NearbyEnemy;
			}
		}	


	}
	else
	{
		float BestDistance = INFINITY;
		for(auto& NearbyEnemy : NearbyEnemies)
		{
			float Distance = FVector::Dist(GetActorLocation(), NearbyEnemy->GetActorLocation());
			if(Distance < BestDistance)
			{
				BestDistance = Distance;
				SuitableTarget = NearbyEnemy;
			}

		}
	}

	if(SuitableTarget != NULL)
	{
		Target = SuitableTarget;

		if(!TargetLocked)
		SetInCombat(true);
	}
}

void AMPlayerCharacter::CycleTargetClockwise()
{
	CycleTarget(true);
}

void AMPlayerCharacter::CycleTargetCounterClockwise()
{
	CycleTarget(false);
}



void AMPlayerCharacter::OnEnemyDetectionBeginOverlap(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if(Cast<AMEnemy_Base>(OtherActor) && NearbyEnemies.Contains(OtherActor))
	{
		NearbyEnemies.Add(OtherActor);
	}
}

void AMPlayerCharacter::OnEnemyDetectionEndOverlap(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex)
{
	if(Cast<AMEnemy_Base>(OtherActor) && NearbyEnemies.Contains(OtherActor))
	{
		NearbyEnemies.Remove(OtherActor);
	}
}


void AMPlayerCharacter::SetInCombat(bool bInCombat)
{
	TargetLocked = bInCombat;

	if(UCharacterMovementComponent* MovementComponent = GetCharacterMovement())
	{
		MovementComponent->MaxWalkSpeed = bInCombat ? CombatMovementSpeed : PassiveMovementSpeed;
		MovementComponent->bOrientRotationToMovement = !bInCombat;
		
		if(!TargetLocked)
		{
			Target = nullptr;
		}
	
	}
}

void AMPlayerCharacter::GetOwnedGameplayTags(FGameplayTagContainer &TagContainer) const
{
	TagContainer = OwnedTags;
}




void AMPlayerCharacter::ToggleCombatMode()
{
	if(!TargetLocked)
	{
		CycleTarget(true);
	}
	else{
		
		SetInCombat(false);
	}
}

void AMPlayerCharacter::FocusTarget()
{
	if(Target != NULL)
	{
		if(FVector::Dist(GetActorLocation(), Target->GetActorLocation()) >= TargetLockDistance)
		{
			ToggleCombatMode();
		}
	}
}

void AMPlayerCharacter::OnHealthChanged(const FOnAttributeChangeData & Data)
{

	float Health = Data.NewValue;
	if(GEngine) GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Red, TEXT("UpdateHealth"));

	HealthChanged(Data.NewValue, Data.OldValue, AttributeSet->GetMaxHealth());
	if(PlayerHUD)
	{ PlayerHUD->SetHealth(Health, AttributeSet->GetMaxHealth());
		if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("UpdateHUD"));
	}
	else{
		if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("CannotUodateHUD"));
	}


	if (Data.NewValue <= 0.0f && !AbilitySystemComponent->HasMatchingGameplayTag(DeathTag))
	{
		Die();
		OnCharacterDeath_BP();
	}
}

void AMPlayerCharacter::OnMaxHealthChanged(const FOnAttributeChangeData & Data)
{
	float MaxHealth = Data.NewValue;

	MaxHealthChanged(Data.NewValue, Data.OldValue, AttributeSet->GetHealth());
	if(PlayerHUD) PlayerHUD->SetHealth(AttributeSet->GetHealth(), MaxHealth);

}

void AMPlayerCharacter::OnEnergyChanged(const FOnAttributeChangeData & Data)
{
	float Energy = Data.NewValue;
	EnergyChanged(Data.NewValue, Data.OldValue, AttributeSet->GetMaxEnergy());
	if(PlayerHUD) PlayerHUD->SetEnergy(Energy, AttributeSet->GetMaxEnergy());
}

void AMPlayerCharacter::OnMaxEnergyChanged(const FOnAttributeChangeData & Data)
{
	float MaxEnergy = Data.NewValue;
	MaxEnergyChanged(Data.NewValue, Data.OldValue, AttributeSet->GetEnergy());
	if(PlayerHUD) PlayerHUD->SetEnergy(AttributeSet->GetEnergy(), MaxEnergy);

}

void AMPlayerCharacter::OnArmorChanged(const FOnAttributeChangeData & Data)
{
	float Armor = Data.NewValue;
	ArmorChanged(Data.NewValue, Data.OldValue, AttributeSet->GetMaxArmor());
	if(PlayerHUD) PlayerHUD->SetArmor(Armor, AttributeSet->GetMaxArmor());
}

void AMPlayerCharacter::OnMaxArmorChanged(const FOnAttributeChangeData & Data)
{
	float MaxArmor = Data.NewValue;
	MaxArmorChanged(Data.NewValue, Data.OldValue, AttributeSet->GetArmor());
	if(PlayerHUD) PlayerHUD->SetArmor(AttributeSet->GetArmor(), MaxArmor);
}

void AMPlayerCharacter::OnLevelChanged(const FOnAttributeChangeData & Data)
{
	int Level = Data.NewValue;
	if(PlayerHUD) PlayerHUD->SetLevel(Level);
}

void AMPlayerCharacter::OnExperienceChanged(const FOnAttributeChangeData & Data)
{
	float Experience = Data.NewValue;
	if(PlayerHUD) PlayerHUD->SetExperience(Experience, 100);
}

int AMPlayerCharacter::GetCharacterLevel()
{

    if(AttributeSet) return AttributeSet->GetLevel();
	else return -1.0f;
}

bool AMPlayerCharacter::IsAlive() const
{
    if(AttributeSet) return AttributeSet->GetHealth() > 0.0f;
	else{
		if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Attempt to call IsAlive on null AttributeSet"));
		return false;
	}
}

