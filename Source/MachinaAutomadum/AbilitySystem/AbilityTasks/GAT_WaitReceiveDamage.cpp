// Fill out your copyright notice in the Description page of Project Settings.


#include "GAT_WaitReceiveDamage.h"

#include "../MAbilitySystemComponent.h"

// Constructor: Sets up the ability task to listen for a damage event
UGAT_WaitReceiveDamage::UGAT_WaitReceiveDamage(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
    // Default values
    //TriggerType = EGameplayAbilityTriggerSource::GameplayEvent;
    //ListenForGameplayEvent(EGameplayEvent::GameplayEvent_Damage, true);
    TriggerOnce = false;
}

/**
// Called when the ability is activated. It checks if the triggering event has the correct tags and if so, stores the instigator (source actor) and ends the ability
void UGAT_WaitReceiveDamage::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    // Store the actor that damaged us
    if (TriggerEventData)
    {
        FGameplayTagContainer DamageTags;
        TriggerEventData->GetAllTags(DamageTags);
        if (DamageTags.HasTagExact(FGameplayTag::RequestGameplayTag(FName("Damage.DamageTag"))))
        {
            FGameplayTagContainer SourceTags;
            TriggerEventData->GetTargetTags(SourceTags);
            if (SourceTags.HasTagExact(FGameplayTag::RequestGameplayTag(FName("Damage.SourceTag"))))
            {
                SourceActor = TriggerEventData->Instigator;
            }
        }
    }

    // If we have a valid source actor, we can end the ability
    if (SourceActor)
    {
        EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
    }
}
// Called when a gameplay event occurs. It checks if the event has the correct tags and if so, stores the instigator (source actor) and ends the ability
void UGAT_WaitReceiveDamage::OnGameplayEvent(FGameplayTag EventTag, const FGameplayEventData* Payload)
{
    Super::OnGameplayEvent(EventTag, Payload);

    // Store the actor that damaged us
    if (Payload)
    {
        FGameplayTagContainer DamageTags;
        Payload->GetAllTags(DamageTags);
        if (DamageTags.HasTagExact(FGameplayTag::RequestGameplayTag(FName("Damage.DamageTag"))))
        {
            FGameplayTagContainer SourceTags;
            Payload->GetTargetTags(SourceTags);
            if (SourceTags.HasTagExact(FGameplayTag::RequestGameplayTag(FName("Damage.SourceTag"))))
            {
                SourceActor = Payload->Instigator;
            }
        }
    }

    // If we have a valid source actor, we can end the ability
    if (SourceActor)
    {
        EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
    }
}
// Checks if the ability can be activated. It can only be activated if there is a valid source actor
bool UGAT_WaitReceiveDamage::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
    // We can only activate this ability if we have a valid source actor
    return SourceActor != nullptr;
}



// Sets the source actor
void UGAT_WaitReceiveDamage::SetSourceActor(AActor* InSourceActor)
{
    SourceActor = InSourceActor;
}

*/

/**
// Static function that creates a new instance of this ability task
UGAT_WaitReceiveDamage* UGAT_WaitReceiveDamage::WaitReceiveDamage(UGameplayAbility* OwningAbility, bool TriggerOnce)
{
    UGAT_WaitReceiveDamage* MyObj = NewAbilityTask<UGAT_WaitReceiveDamage>(OwningAbility);
    MyObj->TriggerOnce = TriggerOnce;
    return MyObj;
}

// Called when damage is received. It broadcasts the OnDamage delegate and ends the ability if TriggerOnce is true
void UGAT_WaitReceiveDamage::OnDamageReceived(UMAbilitySystemComponent* SourceASC, float UnmitigatedDamage, float MitigatedDamage)
{
   if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnDamage.Broadcast(SourceASC, UnmitigatedDamage, MitigatedDamage);
	}

	if (TriggerOnce)
	{
		EndTask();
	}
}

// Calls OnDamageReceived


// Called when the ability task is destroyed. It removes the damage received delegate from the ability system component
void UGAT_WaitReceiveDamage::OnDestroy(bool AbilityEnding)
{
  
    // Clear the source actor
    SourceActor = nullptr;
    UMAbilitySystemComponent* GDASC = Cast<UMAbilitySystemComponent>(AbilitySystemComponent);

	if (GDASC)
	{
		GDASC->ReceivedDamage.RemoveDynamic(this, &UGDAT_WaitReceiveDamage::OnDamageReceived);
	}

	Super::OnDestroy(AbilityIsEnding);

 
}

// Called when the ability task is activated. It adds the damage received delegate to the ability system component
void UGAT_WaitReceiveDamage::Activate()
{
    UMAbilitySystemComponent* GDASC = Cast<UMAbilitySystemComponent>(AbilitySystemComponent);

	if (GDASC)
	{
		GDASC->ReceivedDamage.AddDynamic(this, &UGDAT_WaitReceiveDamage::OnDamageReceived);
	}
}

// Called when damage is received. It broadcasts the OnDamage delegate and ends the task if TriggerOnce is true
void UGAT_WaitReceiveDamage::OnDamageReceived(UMAbilitySystemComponent* SourceASC, float UnmitigatedDamage, float MitigatedDamage)
{
    if (OnDamage.IsBound())
    {
        OnDamage.Broadcast(SourceASC, UnmitigatedDamage, MitigatedDamage);
    }

    if (TriggerOnce)
    {
        EndTask();
    }
}''*/


