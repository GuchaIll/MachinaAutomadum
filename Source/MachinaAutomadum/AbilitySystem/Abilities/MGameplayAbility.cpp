// Fill out your copyright notice in the Description page of Project Settings.


#include "MGameplayAbility.h"
#include "AbilitySystemComponent.h"
#include "../MAbilitySystemComponent.h"

UMGameplayAbility::UMGameplayAbility(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{   
    
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

    ActivationPolicy = EMAbilityActivationPolicy::OnInputTriggered;
	ActivationGroup = EMAbilityActivationGroup::Independent;

    ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("State.Dead")));
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("State.Debuff.Stun")));

    EnergyCost = 0.0f;

}

bool UMGameplayAbility::CanChangeActivationGroup(EMAbilityActivationGroup NewGroup) const
{
    //TODO: Add logic to check if we can change the activation group
    return true;
}

bool UMGameplayAbility::ChangeActivationGroup(EMAbilityActivationGroup NewGroup)
{
    if (CanChangeActivationGroup(NewGroup))
    {
        ActivationGroup = NewGroup;

        //TODO: Add logic to handle changing the activation group, integrate
        return true;
    }

    return false;
}

void UMGameplayAbility::OnAvatarSet(const FGameplayAbilityActorInfo * ActorInfo, const FGameplayAbilitySpec & Spec)
{
	Super::OnAvatarSet(ActorInfo, Spec);

	if (ActivationPolicy == EMAbilityActivationPolicy::OnSpawn && ActorInfo->AbilitySystemComponent.IsValid())
	{
		ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle, false);
	}
}

bool UMGameplayAbility::TryConsumingEnergy(float EnergyToConsume)
{
    
     UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
     if(!ASC)
     {
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("ASC is invalid"));
        } 
        return false;
     }

    if(!EnergyCostEffect)
    { 
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("EnergyCostEffect is not set"));
        }
        return false;

        return false;
    }

      FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(EnergyCostEffect, 1.0f, ASC->MakeEffectContext());
        if (!SpecHandle.IsValid())
    {
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("SpecHandle is invalid"));
        }
        return false;
    }
    
    if (SpecHandle.IsValid())
    {
        // Set the energy cost value
        SpecHandle.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.EnergyCost")), EnergyToConsume); // Example value

        FActiveGameplayEffectHandle GEHandle = ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
        if (!GEHandle.IsValid())
    {
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("GEHandle is invalid"));
        }
        return false;
    }
        
        return GEHandle.IsValid();
    }

    return false;
     
}

void UMGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{

      if (GEngine)
        {
            FString AbilityName = GetName();
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Name: %s"), *AbilityName));
        }

    if (EnergyCost != 0.0f && EnergyCostEffect)
    {
        // Call the parent class's ActivateAbility function
        if(TryConsumingEnergy(EnergyCost))
        {
             Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
        }
          else
        {
        // Not enough energy, end the ability
        EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
         }
    }
       
    else
    {
         Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
    }
}
