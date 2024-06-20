// Fill out your copyright notice in the Description page of Project Settings.


#include "MAttributeSet.h"
#include "Net/UnrealNetwork.h"

//#include "AbilitySystem/MAbilitySystemComponent.h" //TODO

#include UE_INLINE_GENERATED_CPP_BY_NAME(MAttributeSet)

class UWorld;

UMAttributeSet::UMAttributeSet()
{
  
}

void UMAttributeSet::OnRep_TotalCoolDownReduction(const FGameplayAttributeData &OldTotalCoolDownReduction)
{
  GAMEPLAYATTRIBUTE_REPNOTIFY(UMAttributeSet, TotalCoolDownReduction, OldTotalCoolDownReduction);
}

void UMAttributeSet::OnRep_BaseAttackSpeed(const FGameplayAttributeData &OldBaseAttackSpeed)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UMAttributeSet, BaseAttackSpeed, OldBaseAttackSpeed);
}

void UMAttributeSet::OnRep_DamageMultiplier(const FGameplayAttributeData &OldDamageMultiplier)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UMAttributeSet, DamageMultiplier, OldDamageMultiplier);
}


UWorld *UMAttributeSet::GetWorld() const
{
    const UObject* Outer = GetOuter();
    check(Outer);

    return Outer->GetWorld();
}

void UMAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
    {
        Super::GetLifetimeReplicatedProps(OutLifetimeProps);

        DOREPLIFETIME(UMAttributeSet, Health);
        DOREPLIFETIME(UMAttributeSet, MaxHealth);
        DOREPLIFETIME(UMAttributeSet, Energy);
        DOREPLIFETIME(UMAttributeSet, MaxEnergy);
        DOREPLIFETIME(UMAttributeSet, Armor);
        DOREPLIFETIME(UMAttributeSet, MaxArmor);
        DOREPLIFETIME(UMAttributeSet, Level);
        DOREPLIFETIME(UMAttributeSet, Experience);
        DOREPLIFETIME(UMAttributeSet, ExperienceToNextLevel);
        DOREPLIFETIME(UMAttributeSet, Shield);
        DOREPLIFETIME(UMAttributeSet, MaxShield);
        DOREPLIFETIME(UMAttributeSet, ShieldStack);
        DOREPLIFETIME(UMAttributeSet, BaseDamage);
        DOREPLIFETIME(UMAttributeSet, DamageMultiplier);
        DOREPLIFETIME(UMAttributeSet, BaseAttackSpeed);
        DOREPLIFETIME(UMAttributeSet, TotalCoolDownReduction);
    

    }

void UMAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UMAttributeSet, Health, OldHealth);
}

void UMAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UMAttributeSet, MaxHealth, OldMaxHealth);
}

void UMAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldArmor)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UMAttributeSet, Armor, OldArmor);
}

void UMAttributeSet::OnRep_MaxArmor(const FGameplayAttributeData& OldMaxArmor)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UMAttributeSet, MaxArmor, OldMaxArmor);
}

void UMAttributeSet::OnRep_Energy(const FGameplayAttributeData& OldEnergy)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UMAttributeSet, Energy, OldEnergy);
}

void UMAttributeSet::OnRep_MaxEnergy(const FGameplayAttributeData& OldMaxEnergy)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UMAttributeSet, MaxEnergy, OldMaxEnergy);
}

void UMAttributeSet::OnRep_Level(const FGameplayAttributeData& OldLevel)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UMAttributeSet, Level, OldLevel);
}

void UMAttributeSet::OnRep_Experience(const FGameplayAttributeData& OldExperience)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UMAttributeSet, Experience, OldExperience);
}

void UMAttributeSet::OnRep_ExperienceToNextLevel(const FGameplayAttributeData& OldExperienceToNextLevel)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UMAttributeSet, ExperienceToNextLevel, OldExperienceToNextLevel);
}

void UMAttributeSet::OnRep_Shield(const FGameplayAttributeData &OldShield)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UMAttributeSet, Shield, OldShield);
}

void UMAttributeSet::OnRep_BaseDamage(const FGameplayAttributeData &OldBaseDamage)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UMAttributeSet, BaseDamage, OldBaseDamage);
}

void UMAttributeSet::OnRep_ShieldStack(const FGameplayAttributeData &OldShieldStack)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UMAttributeSet, ShieldStack, OldShieldStack);
}

void UMAttributeSet::OnRep_MaxShield(const FGameplayAttributeData &OldMaxShield)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UMAttributeSet, MaxShield, OldMaxShield);
}
