// Fill out your copyright notice in the Description page of Project Settings.


#include "MEnemy_Base.h"
#include "GameplayTagContainer.h"
#include "../AbilitySystem/Attributes/MAttributeSet.h"
#include "../AbilitySystem/MAbilitySystemComponent.h"
#include "../AbilitySystem/Abilities/MGameplayAbility.h"
#include "../Renders/DamageTextRenderComponent.h"
#include "../Renders/Widgets/MFloatingStatusBar.h"
#include "Components/WidgetComponent.h"
#include "kismet/GameplayStatics.h"

// Sets default values
AMEnemy_Base::AMEnemy_Base()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystemComponent = CreateDefaultSubobject<UMAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UMAttributeSet::GetHealthAttribute()).AddUObject(this, &AMEnemy_Base::OnHealthChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UMAttributeSet::GetMaxHealthAttribute()).AddUObject(this, &AMEnemy_Base::OnMaxHealthChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UMAttributeSet::GetEnergyAttribute()).AddUObject(this, &AMEnemy_Base::OnEnergyChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UMAttributeSet::GetMaxEnergyAttribute()).AddUObject(this, &AMEnemy_Base::OnMaxEnergyChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UMAttributeSet::GetArmorAttribute()).AddUObject(this, &AMEnemy_Base::OnArmorChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UMAttributeSet::GetMaxArmorAttribute()).AddUObject(this, &AMEnemy_Base::OnMaxArmorChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UMAttributeSet::GetLevelAttribute()).AddUObject(this, &AMEnemy_Base::OnLevelChanged);
	



	DeathTag = FGameplayTag::RequestGameplayTag(FName("State.Dead"));
	EffectRemoveOnDeathTag = FGameplayTag::RequestGameplayTag(FName("Effect.RemoveOnDeath"));

	//Set up Damage Text 
	DamageTextComponent = CreateDefaultSubobject<UDamageTextRenderComponent>(TEXT("DamageTextComponent"));
	DamageTextComponent->SetupAttachment(RootComponent);

	DamageTextComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 150.0f));
	DamageTextComponent->SetRelativeRotation(FRotator(0.0f, 0.0f, 90.0f));
	DamageTextComponent->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));
	DamageTextComponent->SetHorizontalAlignment(EHTA_Center);
	DamageTextComponent->SetVerticalAlignment(EVRTA_TextCenter);
	DamageTextComponent->SetTextRenderColor(FColor::White);
	DamageTextComponent->SetWorldSize(10.0f);

	//Set up Widget

	UIFloatingStatusBarComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("UIFloatingStatusBarComponent"));
	UIFloatingStatusBarComponent->SetupAttachment(RootComponent);
	UIFloatingStatusBarComponent->SetRelativeLocation(FVector(0.0f, -50.0f, 120.0f));
	UIFloatingStatusBarComponent->SetWidgetSpace(EWidgetSpace::Screen);
	UIFloatingStatusBarComponent->SetDrawSize(FVector2D(25.0f, 25.0f));

	
	UIFloatingStatusBarClass = StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("/Game/MachinaAutomadum/Renders/Widgets/W_FloatingStatusBar.W_FloatingStatusBar_C"));
	if (!UIFloatingStatusBarClass)
	{
		UE_LOG(LogTemp, Error, TEXT("%s() Failed to find UIFloatingStatusBarClass. If it was moved, please update the reference location in C++."), *FString(__FUNCTION__));
	}



}

// Called when the game starts or when spawned
void AMEnemy_Base::BeginPlay()
{
	Super::BeginPlay();
	
	//Set up initial values for attributes
	if(IsValid(AbilitySystemComponent))
	{
		AttributeSet = AbilitySystemComponent->GetSet<UMAttributeSet>();
		if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Enemy Attribute Set is valid"));
	}
	else{
		if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Enemy Attribute Set is not valid"));
	}

	//Give Ability to Character
	for(TSubclassOf<UMGameplayAbility> Ability : CharacterAbilities)
	{
		AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(Ability, 1, 0));
	}

	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (PC && PC->IsLocalPlayerController())
		{
			if (UIFloatingStatusBarClass)
			{
				UIFloatingStatusBar = CreateWidget<UMFloatingStatusBar>(PC, UIFloatingStatusBarClass);
				if (UIFloatingStatusBar && UIFloatingStatusBarComponent)
				{
					UIFloatingStatusBarComponent->SetWidget(UIFloatingStatusBar);

					// Setup the floating status bar
					UIFloatingStatusBar->SetHealth(AttributeSet->GetHealth(), AttributeSet->GetMaxHealth());
					UIFloatingStatusBar->SetEnergy(AttributeSet->GetEnergy(), AttributeSet->GetMaxEnergy());
					UIFloatingStatusBar->SetArmor(AttributeSet->GetArmor(), AttributeSet->GetMaxArmor());
					UIFloatingStatusBar->SetLevel(AttributeSet->GetLevel());
				}
			}
		}

}


// Called every frame
void AMEnemy_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


UAbilitySystemComponent* AMEnemy_Base::GetAbilitySystemComponent() const
{
    // Return the ability system component
    return AbilitySystemComponent;
}

void AMEnemy_Base::OnHealthChanged(const FOnAttributeChangeData& Data)
{
	float Health = Data.NewValue;
	
	if(UIFloatingStatusBar)
	{
		UIFloatingStatusBar->SetHealth(Health, AttributeSet->GetMaxHealth());
	}

	if(Health <= 0.0f && !AbilitySystemComponent->HasMatchingGameplayTag(DeathTag))
	{
		//Perform Death Related Actions
		AbilitySystemComponent->AddLooseGameplayTag(DeathTag);

		FGameplayTagContainer EffectRemoveOnDeathTagContainer;
		EffectRemoveOnDeathTagContainer.AddTag(EffectRemoveOnDeathTag);
		AbilitySystemComponent->RemoveActiveEffectsWithGrantedTags(EffectRemoveOnDeathTagContainer);

		EnemyDeath();
	}
}

void AMEnemy_Base::UpdateDamageText(float Damage)
{
	if(DamageTextComponent)
	{
		DamageTextComponent->SetText(FText::AsNumber(Damage));
	}
}

void AMEnemy_Base::GetOwnedGameplayTags(FGameplayTagContainer &TagContainer) const
{
	TagContainer = OwnedTags;
}

void AMEnemy_Base::DisplayDamageText()
{
	if(DamageTextComponent)
	{
		DamageTextComponent->SetVisibility(DamageTextComponent->IsVisible() ? false : true);
	}
}

void AMEnemy_Base::OnMaxHealthChanged(const FOnAttributeChangeData& Data)
{
	float MaxHealth = Data.NewValue;

	if(UIFloatingStatusBar)
	{
		UIFloatingStatusBar->SetHealth(AttributeSet->GetHealth(), MaxHealth);
	}
}

void AMEnemy_Base::OnEnergyChanged(const FOnAttributeChangeData& Data)
{

	float Energy = Data.NewValue;
	if(UIFloatingStatusBar)
	{
		UIFloatingStatusBar->SetEnergy(Energy, AttributeSet->GetMaxEnergy());
	}
}

void AMEnemy_Base::OnMaxEnergyChanged(const FOnAttributeChangeData& Data)
{

	float MaxEnergy = Data.NewValue;
	if(UIFloatingStatusBar)
	{
		UIFloatingStatusBar->SetEnergy(AttributeSet->GetEnergy(), MaxEnergy);
	}
}

void AMEnemy_Base::OnArmorChanged(const FOnAttributeChangeData& Data)
{
	float Armor = Data.NewValue;
	if(UIFloatingStatusBar)
	{
		UIFloatingStatusBar->SetArmor(Armor, AttributeSet->GetMaxArmor());
	}
}

void AMEnemy_Base::OnMaxArmorChanged(const FOnAttributeChangeData& Data)
{
	float MaxArmor = Data.NewValue;
	if(UIFloatingStatusBar)
	{
		UIFloatingStatusBar->SetArmor(AttributeSet->GetArmor(), MaxArmor);
	}

}


void AMEnemy_Base::OnLevelChanged(const FOnAttributeChangeData& Data)
{
	int Level = Data.NewValue;
	if(UIFloatingStatusBar)
	{
		UIFloatingStatusBar->SetLevel(Level);
	}
}

