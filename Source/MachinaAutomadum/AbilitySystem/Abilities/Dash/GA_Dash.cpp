// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_Dash.h"
#include "../../MAbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AbilitySystemGlobals.h"
#include "../../../Core/MPlayerCharacter.h"

UGA_Dash::UGA_Dash()
{

    // Default values
    DashDistance = 1000.0f;
    DashSpeed = 1000.0f;
    DashCooldown = 2.0f;
   

    FGameplayTag ImmunityTag = FGameplayTag::RequestGameplayTag(FName("State.Immunity"));
    FGameplayTag AbilityTag = FGameplayTag::RequestGameplayTag(FName("Ability.Skill.Dash"));
    AbilityTags.AddTag(AbilityTag);        
    ActivationOwnedTags.AddTag(ImmunityTag);

     ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Skill")));
}

void UGA_Dash::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, TEXT("Dash Activated"));
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    if (CommitAbility(Handle, ActorInfo, ActivationInfo))
    {
        
        // Get the owning character
        AMPlayerCharacter* Character = Cast<AMPlayerCharacter>(ActorInfo->AvatarActor.Get());
        if (Character)
        {
            // Get the character movement component
            UCharacterMovementComponent* CharacterMovement = Character->GetCharacterMovement();
            if (CharacterMovement)
            {
                // Get the forward vector of the character
                FVector ForwardVector = Character->GetActorForwardVector();
                ForwardVector.Z = 0.0f;
                ForwardVector.Normalize();

                // Calculate the dash location
                FVector DashLocation = Character->GetActorLocation() + ForwardVector * DashDistance;

                // Set the character's location to the dash location
                Character->SetActorLocation(DashLocation);

                // Set the character's velocity to the dash speed
                CharacterMovement->Velocity = ForwardVector * DashSpeed;

                if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, CharacterMovement->Velocity.ToString());


               
                EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);


                // Set the cooldown
                //SetCooldown(Handle, ActorInfo, ActivationInfo, this, DashCooldown);
            }
        }
    }
}






