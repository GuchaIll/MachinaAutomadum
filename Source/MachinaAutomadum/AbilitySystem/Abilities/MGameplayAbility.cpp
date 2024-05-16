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

