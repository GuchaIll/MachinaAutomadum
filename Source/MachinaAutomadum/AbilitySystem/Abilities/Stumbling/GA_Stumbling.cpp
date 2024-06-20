#include "CoreMinimal.h"
#include "GA_Stumbling.h"
#include "AbilitySystemGlobals.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../../MAbilitySystemComponent.h"


UGA_Stumbling::UGA_Stumbling()
{
    // Default values
    StumbleDistance = 50.0f;
    StumbleSpeed = 100.0f;
    StumbleCooldown = 0.5f;

    //FGameplayTag ImmunityTag = FGameplayTag::RequestGameplayTag(FName("State.Immunity"));
    //FGameplayTag AbilityTag = FGameplayTag::RequestGameplayTag(FName("Ability.Skill.Stumbling"));
    //AbilityTags.AddTag(AbilityTag);
    //ActivationOwnedTags.AddTag(ImmunityTag);

    //ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Skill")));
}

void UGA_Stumbling::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    if (CommitAbility(Handle, ActorInfo, ActivationInfo))
    {
        if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, TEXT("Stumbling Activated"));

        // Get the owning character
        ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
        if (Character)
        {
            
            FVector HitDirection = Character->GetActorForwardVector();
            Character->LaunchCharacter(-HitDirection * StumbleSpeed, true, true);

            UAnimMontage* Montage = GetStumbleMontage(HitDirection);

            if(Montage)
            {
              UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
                if(AnimInstance)
                {
                    AnimInstance->Montage_Play(Montage);
                }
            }
        }
    }
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
    return;
}

UAnimMontage* UGA_Stumbling::GetStumbleMontage(FVector HitDirection)
{
    if (HitDirection.X > 0.5f)
    {
        return StumbleRightMontage;
    }
    else if (HitDirection.X < -0.5f)
    {
        return StumbleLeftMontage;
    }
    else if (HitDirection.Y > 0.5f)
    {
        return StumbleForwardMontage;
    }
    else if (HitDirection.Y < -0.5f)
    {
        return StumbleBackwardMontage;
    }
    else if (HitDirection.Z > 0.5f)
    {
        return StumbleUpwardMontage;
    }
    else if (HitDirection.Z < -0.5f)
    {
        return StumbleDownwardMontage;
    }
    else
    {
        return nullptr;
    }
}