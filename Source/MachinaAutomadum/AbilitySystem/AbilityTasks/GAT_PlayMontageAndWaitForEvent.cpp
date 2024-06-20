// Fill out your copyright notice in the Description page of Project Settings.


#include "GAT_PlayMontageAndWaitForEvent.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Animation/AnimInstance.h"
#include  "../MAbilitySystemComponent.h"
#include "../Abilities/MGameplayAbility.h"
#include "GameFramework/Character.h"


UGAT_PlayMontageAndWaitForEvent::UGAT_PlayMontageAndWaitForEvent(const FObjectInitializer& ObjectInitializer)
{
    Rate = 1.f;
    bStopWhenAbilityEnds = true;
    bRootMotionLock = false;
}

void UGAT_PlayMontageAndWaitForEvent::OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted)
{
    if(Ability && Ability->GetCurrentMontage() == MontageToPlay)
    {
        if(Montage == MontageToPlay)
        {
            AbilitySystemComponent->ClearAnimatingAbility(Ability);

            ACharacter* Character = Cast<ACharacter>(AbilitySystemComponent->GetAvatarActor());
            if(Character)
            {
                Character->SetAnimRootMotionTranslationScale(1.f);

            }
        }
    }

    if(bInterrupted)
    {
        if (ShouldBroadcastAbilityTaskDelegates())
        {
            OnInterrupted.Broadcast(FGameplayTag(), FGameplayEventData());
        }
    }
    else{
        if (ShouldBroadcastAbilityTaskDelegates())
        {
            OnBlendOut.Broadcast(FGameplayTag(), FGameplayEventData());
        }
    }
}

void UGAT_PlayMontageAndWaitForEvent::OnAbilityCancelled()
{
    if(StopPlayingMontage())
    {
        if (ShouldBroadcastAbilityTaskDelegates())
        {
            OnCancelled.Broadcast(FGameplayTag(), FGameplayEventData());
        }
    }
}

void UGAT_PlayMontageAndWaitForEvent::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
    if(!bInterrupted)
    {
        if (ShouldBroadcastAbilityTaskDelegates())
        {
            OnCompleted.Broadcast(FGameplayTag(), FGameplayEventData());
        }
    }

    EndTask();

}

void UGAT_PlayMontageAndWaitForEvent::OnGameplayEvent(FGameplayTag EventTag, const FGameplayEventData* Payload)
{
    if (ShouldBroadcastAbilityTaskDelegates())
    {
        FGameplayEventData TempData = *Payload;
        TempData.EventTag = EventTag;

        EventReceived.Broadcast(EventTag, TempData);
    }
}

UGAT_PlayMontageAndWaitForEvent* UGAT_PlayMontageAndWaitForEvent::PlayMontageAndWaitForEvent(UGameplayAbility* OwningAbility, FName InTaskInstanceName,
UAnimMontage* InMontageToPlay, FGameplayTagContainer InEventTags, float InRate, FName InStartSection, bool bInStopWhenAbilityEnds, float InAnimRootMotionTranslationScale)
{
    UAbilitySystemGlobals::NonShipping_ApplyGlobalAbilityScaler_Duration(InRate);

    UGAT_PlayMontageAndWaitForEvent* MyObj = NewAbilityTask<UGAT_PlayMontageAndWaitForEvent>(OwningAbility, InTaskInstanceName);
    MyObj->MontageToPlay = InMontageToPlay;
    MyObj->EventTags = InEventTags;
    MyObj->Rate = InRate;
    MyObj->StartSection = InStartSection;
    MyObj->AnimRootMotionTranslationScale = InAnimRootMotionTranslationScale;
    MyObj->bStopWhenAbilityEnds = bInStopWhenAbilityEnds;

    return MyObj;

}

void UGAT_PlayMontageAndWaitForEvent::Activate()
{
    if (Ability == nullptr)
    {
        return;
    }

    bool bPlayedMontage = false;

    if (AbilitySystemComponent.IsValid())
    {
        const FGameplayAbilityActorInfo* ActorInfo = Ability->GetCurrentActorInfo();
        UAnimInstance* AnimInstance = ActorInfo->GetAnimInstance();
        if (AnimInstance != nullptr)
        {
            EventHandle = AbilitySystemComponent->AddGameplayEventTagContainerDelegate(EventTags, FGameplayEventTagMulticastDelegate::FDelegate::CreateUObject(this, &UGAT_PlayMontageAndWaitForEvent::OnGameplayEvent));

            if (AbilitySystemComponent->PlayMontage(Ability, Ability->GetCurrentActivationInfo(), MontageToPlay, Rate, StartSection) > 0.f)
            {

                if(ShouldBroadcastAbilityTaskDelegates() == false)
                {
                    return;
                }

                CancelledHandle = Ability->OnGameplayAbilityCancelled.AddUObject(this, &UGAT_PlayMontageAndWaitForEvent::OnAbilityCancelled);

                BlendingOutDelegate.BindUObject(this, &UGAT_PlayMontageAndWaitForEvent::OnMontageBlendingOut);
                AnimInstance->Montage_SetBlendingOutDelegate(BlendingOutDelegate, MontageToPlay);

                MontageEndedDelegate.BindUObject(this, &UGAT_PlayMontageAndWaitForEvent::OnMontageEnded);
                AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, MontageToPlay);

                ACharacter* Character = Cast<ACharacter>(GetAvatarActor());
                if (Character)
                {
                    Character->SetAnimRootMotionTranslationScale(AnimRootMotionTranslationScale);
                }

                  bPlayedMontage = true;
            }   
            
        }
        else{
            UE_LOG(LogTemp, Warning, TEXT("UGDAbilityTask_PlayMontageAndWaitForEvent call to PlayMontage failed!"));

        }
    }
    else{
        UE_LOG(LogTemp, Warning, TEXT("UGDAbilityTask_PlayMontageAndWaitForEvent called on invalid AbilitySystemComponent"));
    }

    if(!bPlayedMontage)
    {
        UE_LOG(LogTemp, Warning, TEXT("UGDAbilityTask_PlayMontageAndWaitForEvent called in Ability %s failed to play montage %s; Task Instance Name %s."), *Ability->GetName(), *GetNameSafe(MontageToPlay), *InstanceName.ToString());
        if(ShouldBroadcastAbilityTaskDelegates())
        {
            OnCancelled.Broadcast(FGameplayTag(), FGameplayEventData());
        }
    }
    SetWaitingOnAvatar();
}

void UGAT_PlayMontageAndWaitForEvent::ExternalCancel()
{
    check(AbilitySystemComponent.IsValid());
    OnAbilityCancelled();
    Super::ExternalCancel();
}

 void UGAT_PlayMontageAndWaitForEvent::OnDestroy(bool AbilityEnded)
{
	// Note: Clearing montage end delegate isn't necessary since its not a multicast and will be cleared when the next montage plays.
	// (If we are destroyed, it will detect this and not do anything)

	// This delegate, however, should be cleared as it is a multicast
	if (Ability)
	{
		Ability->OnGameplayAbilityCancelled.Remove(CancelledHandle);
		if (AbilityEnded && bStopWhenAbilityEnds)
		{
			StopPlayingMontage();
		}
	}

	if (AbilitySystemComponent.IsValid())
	{
		AbilitySystemComponent->RemoveGameplayEventTagContainerDelegate(EventTags, EventHandle);
	}

	Super::OnDestroy(AbilityEnded);

}

bool UGAT_PlayMontageAndWaitForEvent::StopPlayingMontage()
{
	const FGameplayAbilityActorInfo* ActorInfo = Ability->GetCurrentActorInfo();
	if (!ActorInfo)
	{
		return false;
	}

	UAnimInstance* AnimInstance = ActorInfo->GetAnimInstance();
	if (AnimInstance == nullptr)
	{
		return false;
	}

	// Check if the montage is still playing
	// The ability would have been interrupted, in which case we should automatically stop the montage
	if (AbilitySystemComponent.IsValid() && Ability)
	{
		if (AbilitySystemComponent->GetAnimatingAbility() == Ability
			&& AbilitySystemComponent->GetCurrentMontage() == MontageToPlay)
		{
			// Unbind delegates so they don't get called as well
			FAnimMontageInstance* MontageInstance = AnimInstance->GetActiveInstanceForMontage(MontageToPlay);
			if (MontageInstance)
			{
				MontageInstance->OnMontageBlendingOutStarted.Unbind();
				MontageInstance->OnMontageEnded.Unbind();
			}

			AbilitySystemComponent->CurrentMontageStop();
			return true;
		}
	}

	return false;
}

FString UGAT_PlayMontageAndWaitForEvent::GetDebugString() const
{
	UAnimMontage* PlayingMontage = nullptr;
	if (Ability)
	{
		const FGameplayAbilityActorInfo* ActorInfo = Ability->GetCurrentActorInfo();
		UAnimInstance* AnimInstance = ActorInfo->GetAnimInstance();

		if (AnimInstance != nullptr)
		{
			PlayingMontage = AnimInstance->Montage_IsActive(MontageToPlay) ? MontageToPlay : AnimInstance->GetCurrentActiveMontage();
		}
	}

	return FString::Printf(TEXT("PlayMontageAndWaitForEvent. MontageToPlay: %s  (Currently Playing): %s"), *GetNameSafe(MontageToPlay), *GetNameSafe(PlayingMontage));
}