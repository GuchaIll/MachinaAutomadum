// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_BaseTriggeredInputAction.h"
#include "../../Core/MPlayerCharacter.h"
#include "EnhancedInputComponent.h"

UGA_BaseTriggeredInputAction::UGA_BaseTriggeredInputAction(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    bCancelAbilityOnInputRelease = true;
}

void  UGA_BaseTriggeredInputAction::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
  
  {  Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    bool bSuccess = false;

    if (const AMPlayerCharacter* PlayerCharacter = Cast<AMPlayerCharacter>(GetAvatarActorFromActorInfo()))
    {
        EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerCharacter->InputComponent);
        if (EnhancedInputComponent)
        {
           if(const UPlayerGameplayAbilitiesDataAsset* PlayerGameplayAbilitiesDataAsset = PlayerCharacter->GetPlayerGameplayAbilitiesDataAsset())
           {
             const TSet<FGameplayInputAbilityInfo>& InputAbilities = PlayerGameplayAbilitiesDataAsset->GetInputAbilities();
             for (const auto& It : InputAbilities)
             {
                if (It.IsValid() && It.GameplayAbilityClass == GetClass())
                {
                   const FEnhancedInputActionEventBinding& TriggeredEventBinding = EnhancedInputComponent->BindAction(It.InputAction, 
                    ETriggerEvent::Triggered, this, 
                    &UGA_BaseTriggeredInputAction::OnTriggeredInputAction);
                    const uint32 TriggeredEventHandle = TriggeredEventBinding.GetHandle();

                    TriggeredEventHandles.Add(TriggeredEventHandle);

                    bSuccess = true;
                
                }
             }
           }
        }
    }
    if (bSuccess)
    {
        CommitAbility(Handle, ActorInfo, ActivationInfo);
    }
    else{
        constexpr bool bReplicatedCancelAbility = true;
        CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicatedCancelAbility);
    }
}

void UGA_BaseTriggeredInputAction::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

    if (EnhancedInputComponent)
    {
        for (uint32 TriggeredEventHandle : TriggeredEventHandles)
        {
            EnhancedInputComponent->RemoveBindingByHandle(TriggeredEventHandle);
        }

        EnhancedInputComponent = nullptr;;
    
    }

    TriggeredEventHandles.Reset();
}

void  UGA_BaseTriggeredInputAction::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) 
{
    Super::InputReleased(Handle, ActorInfo, ActivationInfo);
    if (bCancelAbilityOnInputRelease)
    {
        CancelAbility(Handle, ActorInfo, ActivationInfo, true);
    }
}

void UGA_BaseTriggeredInputAction::OnTriggeredInputAction_Implementation(const FInputActionValue& Value)
{
   
}