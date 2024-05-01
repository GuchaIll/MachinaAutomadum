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
#include "InputActionValue.h"
#include "DrawDebugHelpers.h"


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

}

// Called when the game starts or when spawned
void AMPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(MCMappingContext, 0);
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
	}
	else
	{
		
	}
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
	if(!bIsDashing)
	{
	
		bIsDashing = true;
		//Calculate the dash offset
		FVector DashOffset = GetActorForwardVector() * DashDistance;

		AddActorWorldOffset(DashOffset, true, nullptr, ETeleportType::TeleportPhysics);

		GetWorldTimerManager().SetTimer(DashTimerHandle, this, &AMPlayerCharacter::StopDashing, 1.0f, false);
		
	}
}

void AMPlayerCharacter::StopDashing()
{
	bIsDashing = false;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
}

void AMPlayerCharacter::Jump()
{
	if (GetCharacterMovement()->IsFalling() == false || JumpCount < MaxJumpCount)
	{
		
		//Super::Jump();
		 // Calculate the jump velocity

		FVector Velocity = GetVelocity();
		Velocity.Z = 0;
		GetCharacterMovement()->Velocity = Velocity;

        FVector JumpVelocity = FVector(0, 0, GetCharacterMovement()->JumpZVelocity);
        
        // Launch the character into the air
        LaunchCharacter(JumpVelocity, false, false);

		JumpCount++;
	}
}

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