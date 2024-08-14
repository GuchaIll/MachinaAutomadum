#include "MCharacter.h"
#include "GameplayTagContainer.h"
#include "../AbilitySystem/Attributes/MAttributeSet.h"
#include "../AbilitySystem/MAbilitySystemComponent.h"
#include "../AbilitySystem/Abilities/MGameplayAbility.h"
#include "kismet/GameplayStatics.h"

AMCharacter::AMCharacter()
{

    PrimaryActorTick.bCanEverTick = true;

	AbilitySystemComponent = CreateDefaultSubobject<UMAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

    AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UMAttributeSet::GetHealthAttribute()).AddUObject(this, &AMCharacter::OnHealthChanged);
    AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UMAttributeSet::GetMaxHealthAttribute()).AddUObject(this, &AMCharacter::OnMaxHealthChanged);
    AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UMAttributeSet::GetEnergyAttribute()).AddUObject(this, &AMCharacter::OnEnergyChanged);
    AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UMAttributeSet::GetMaxEnergyAttribute()).AddUObject(this, &AMCharacter::OnMaxEnergyChanged);
    AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UMAttributeSet::GetArmorAttribute()).AddUObject(this, &AMCharacter::OnArmorChanged);
    AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UMAttributeSet::GetMaxArmorAttribute()).AddUObject(this, &AMCharacter::OnMaxArmorChanged);
    AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UMAttributeSet::GetLevelAttribute()).AddUObject(this, &AMCharacter::OnLevelChanged);
    AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UMAttributeSet::GetShieldAttribute()).AddUObject(this, &AMCharacter::OnShieldChanged);
    AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UMAttributeSet::GetMaxShieldAttribute()).AddUObject(this, &AMCharacter::OnMaxShieldChanged);
    AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UMAttributeSet::GetShieldStackAttribute()).AddUObject(this, &AMCharacter::OnShieldStackChanged);
    AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UMAttributeSet::GetBaseDamageAttribute()).AddUObject(this, &AMCharacter::OnBaseDamageChanged);

    DeathTag = FGameplayTag::RequestGameplayTag(FName("State.Dead"));
}

UAbilitySystemComponent *AMCharacter::GetAbilitySystemComponent() const
{
    return AbilitySystemComponent;
}

void AMCharacter::BeginPlay()
{
    Super::BeginPlay();


    for(TSubclassOf<UMGameplayAbility> Ability : DefaultAbilities)
	{
		AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(Ability, 1, 0));
	}

    for(TSubclassOf<UMGameplayAbility> Ability : CharacterAbilities)
	{
		AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(Ability, 1, 0));
	}

    for(TSubclassOf<UGameplayEffect> Effect : StartupEffects)
    {
        FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
        EffectContext.AddSourceObject(this);

        FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(Effect, 1, EffectContext);
        if(NewHandle.IsValid())
        {
            FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*NewHandle.Data.Get());
        }
    }

    


   
    
}

void AMCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}


void AMCharacter::OnHealthChanged(const FOnAttributeChangeData &Data)
{
     Health = Data.NewValue;

    if(Health <= 0.0f && !AbilitySystemComponent->HasMatchingGameplayTag(DeathTag))
	{
		//Perform Death Related Actions
		OwnedTags.AddTag(DeathTag);

		OnDeath();
	}

}

void AMCharacter::OnMaxHealthChanged(const FOnAttributeChangeData &Data)
{
    MaxHealth = Data.NewValue;

}

void AMCharacter::OnEnergyChanged(const FOnAttributeChangeData &Data)
{
    Energy = Data.NewValue;
}

void AMCharacter::OnMaxEnergyChanged(const FOnAttributeChangeData &Data)
{
    MaxEnergy = Data.NewValue;
}

void AMCharacter::OnArmorChanged(const FOnAttributeChangeData &Data)
{
    Armor = Data.NewValue;
}

void AMCharacter::OnMaxArmorChanged(const FOnAttributeChangeData &Data)
{   
    MaxArmor = Data.NewValue;
}

void AMCharacter::OnLevelChanged(const FOnAttributeChangeData &Data)
{
    Level = Data.NewValue;

}

void AMCharacter::OnShieldChanged(const FOnAttributeChangeData &Data)
{
    Shield = Data.NewValue;
}

void AMCharacter::OnMaxShieldChanged(const FOnAttributeChangeData &Data)
{
    MaxShield = Data.NewValue;
}

void AMCharacter::OnShieldStackChanged(const FOnAttributeChangeData &Data)
{
    ShieldStack = Data.NewValue;
}

void AMCharacter::OnBaseDamageChanged(const FOnAttributeChangeData &Data)
{
    BaseDamage = Data.NewValue;
}

void AMCharacter::OnDeath()
{
    
}

void AMCharacter::OnDamageTaken(const FOnAttributeChangeData &Data)
{
}

void AMCharacter::GetOwnedGameplayTags(FGameplayTagContainer &TagContainer) const
{
    TagContainer = OwnedTags;
}
