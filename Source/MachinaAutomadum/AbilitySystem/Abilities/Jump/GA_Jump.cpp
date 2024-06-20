
#include "GA_Jump.h"
#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "../../MAbilitySystemComponent.h"
#include "../MGameplayAbility.h"
#include "../../../Core/MPlayerCharacter.h"

UGA_Jump::UGA_Jump()
{
    // Default values
    JumpDistance = 100.0f;
    JumpHeight = 2.0f;
    JumpCooldown = 2.0f;

    //
    FGameplayTag ImmunityTag = FGameplayTag::RequestGameplayTag(FName("State.Immunity.CloseRange"));
    FGameplayTag AbilityTag = FGameplayTag::RequestGameplayTag(FName("Ability.Skill.Jump"));
    AbilityTags.AddTag(AbilityTag);
    ActivationOwnedTags.AddTag(ImmunityTag);

    ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Skill")));
}

void UGA_Jump::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, TEXT("Jump Activated"));
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

                if(CharacterMovement->IsFalling() == false || Character->JumpCount < Character->MaxJumpCount)
                {
                    FVector ForwardVector = Character->GetActorForwardVector();
                    ForwardVector.Z = 0.0f;
                    ForwardVector.Normalize();

                    FVector InputVector = Character->GetLastMovementInputVector();
                    InputVector.Z = 0.0f;
                    InputVector.Normalize();

                    FVector JumpVelocity = CharacterMovement->Velocity;

                    JumpVelocity.Z = CharacterMovement->JumpZVelocity;

                    Character->LaunchCharacter(JumpVelocity, true, true);

                    Character->JumpCount++;
                }
                else
                {
                    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
                    return;
                }

                // Calculate the jump location
                //FVector JumpLocation = Character->GetActorLocation() + FVector(0.0f, 0.0f, JumpHeight);

                // Set the character's location to the jump location
                //Character->SetActorLocation(JumpLocation);

                // Set the character's velocity to the jump speed
                //haracterMovement->Velocity = ForwardVector * JumpDistance;

                GetWorld()->GetTimerManager().SetTimer(JumpTimerHandle, this, &UGA_Jump::ResetTimer, 1.0f, false);

                if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, CharacterMovement->Velocity.ToString());

                EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
            }
            else
                {
                    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
                    return;
                }

        }
        else
                {
                    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
                    return;
                }

    }
}

void UGA_Jump::ResetTimer()
{
    AActor* Avatar = GetAvatarActorFromActorInfo();
    if(Avatar)
    {
        AMPlayerCharacter* Character = Cast<AMPlayerCharacter>(Avatar);

        if (Character)
        {
             Character->JumpCount = 0;
        }
    }
    
   
    GetWorld()->GetTimerManager().ClearTimer(JumpTimerHandle);
}