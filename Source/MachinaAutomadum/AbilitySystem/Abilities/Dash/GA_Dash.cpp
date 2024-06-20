// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_Dash.h"
#include "../../MAbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AbilitySystemGlobals.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "../../AbilityTasks/GAT_PlayMontageAndWaitForEvent.h"
#include "../../../Core/MPlayerCharacter.h"

UGA_Dash::UGA_Dash()
{

    // Default values
    DashDistance = 5000.0f;
    TimeScaleFactor = 1.0f;
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
                CharacterMovement->MaxWalkSpeed = 680.0f;

              
                //Character->AddMovementInput( ForwardVector, 600.0f);

                // Set the character's velocity to the dash speed
                //CharacterMovement->Velocity = ForwardVector * DashSpeed;

                if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, CharacterMovement->Velocity.ToString());

                CharacterMovement->MaxWalkSpeed = 500.0f;

               //Set the character's rotation based on input
                FVector InputDirection = Character->InputDirection;

                FRotator DashRotation;
                FVector DashDirection;
                if(!InputDirection.IsNearlyZero())
                {
                   FRotator PlayerRotationZeroPitch = Character->GetController()->GetControlRotation();
                     PlayerRotationZeroPitch.Pitch = 0.0f;

                    FVector PlayerRight = FRotationMatrix(PlayerRotationZeroPitch).GetUnitAxis(EAxis::Y);
                    FVector PlayerForward = FRotationMatrix(PlayerRotationZeroPitch).GetUnitAxis(EAxis::X);

                     DashDirection =  PlayerRight * InputDirection.Y + PlayerForward * InputDirection.X;
                    DashRotation = DashDirection.ToOrientationRotator(); 

                }
                else
                   DashRotation = Character->GetActorRotation();
                
                Character->SetActorRotation(DashRotation);
                //FVector ForwardVector = Character->GetActorForwardVector();

                //ForwardVector.Z = 0.0f;
                //ForwardVector.Normalize();

                if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::SanitizeFloat(DashDistance * GetWorld()->GetDeltaSeconds() * TimeScaleFactor));
                //Character->AddActorWorldOffset(ForwardVector * DashDistance * TimeScaleFactor, true, nullptr, ETeleportType::TeleportPhysics);
                Character->LaunchCharacter(DashDirection * DashDistance * TimeScaleFactor, true, true);

                UGAT_PlayMontageAndWaitForEvent* Task = UGAT_PlayMontageAndWaitForEvent::PlayMontageAndWaitForEvent(this, NAME_None, RollMontage, FGameplayTagContainer(), 1.0f, NAME_None, false, 1.0f);
                Task->OnBlendOut.AddDynamic(this, &UGA_Dash::OnDashComplete);
                Task->OnCompleted.AddDynamic(this, &UGA_Dash::OnDashComplete);
                Task->OnInterrupted.AddDynamic(this, &UGA_Dash::OnInterrupted);
                Task->OnCancelled.AddDynamic(this, &UGA_Dash::OnCancelled);
                Task->EventReceived.AddDynamic(this, &UGA_Dash::EventRecieved);
                Task->ReadyForActivation();

                // Get the forward vector of the character
              //AddActorWorldOffset(DashOffset, true, nullptr, ETeleportType::TeleportPhysics);
                EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);


                // Set the cooldown
                //SetCooldown(Handle, ActorInfo, ActivationInfo, this, DashCooldown);
                
            }
        }
    }
}

void UGA_Dash::EventRecieved(FGameplayTag EventTag, FGameplayEventData EventData)
{
    if (EventTag == FGameplayTag::RequestGameplayTag(FName("Event.Montage.EndAbility")))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}
}

void UGA_Dash::OnInterrupted(FGameplayTag EventTag, FGameplayEventData EventData)
{
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UGA_Dash::OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UGA_Dash::OnRollComplete(FGameplayTag EventTag, FGameplayEventData EventData)
{
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UGA_Dash::OnDashComplete(FGameplayTag EventTag, FGameplayEventData EventData)
{
    //Execute the dash after roll
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}






