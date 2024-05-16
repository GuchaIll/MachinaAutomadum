// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_ProjectilePoke.h"
#include "../../MAbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "../../../Core/MPlayerCharacter.h"
#include "../../AbilityTasks/GAT_PlayMontageAndWaitForEvent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"

UGA_ProjectilePoke::UGA_ProjectilePoke()
{
    //InstancingPolicy = EGameplayAbilitInstancingPolicy::InstancedPerActor;
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Ability called"));

    FGameplayTag AbilityTag = FGameplayTag::RequestGameplayTag(FName("Ability.Poke.Projectile"));
    AbilityTags.AddTag(AbilityTag);
    ActivationOwnedTags.AddTag(AbilityTag);

    ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Skill")));

    Range = 1000.0f;
    Damage = 10.0f;

}

void UGA_ProjectilePoke::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData *TriggerEventData)
{

      GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, TEXT("Activate called"));

    if(!CommitAbility(Handle, ActorInfo, ActivationInfo))
    {
        EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
        return;
    }
     EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
     return;

     

    //if(AttemptPokeMontage)
   // {B
        //UGAT_PlayMontageAndWaitForEvent* Task = UGAT_PlayMontageAndWaitForEvent::PlayMontageAndWaitForEvent(this, NAME_None, MontageToPlay, FGameplayTagContainer(), 1.0f, NAME_None, false, 1.0f);
	    //Task->OnBlendOut.AddDynamic(this, &UGA_ProjectilePoke::OnCompleted);
        //Task->OnCompleted.AddDynamic(this, &UGA_ProjectilePoke::OnCompleted);
        //Task->OnInterrupted.AddDynamic(this, &UGA_ProjectilePoke::OnCancelled);
        //Task->OnCancelled.AddDynamic(this, &UGA_ProjectilePoke::OnCancelled);
        //Task->EventReceived.AddDynamic(this, &UGA_ProjectilePoke::EventReceived);
        //Task->ReadyForActivation();
    //}

}

void UGA_ProjectilePoke::OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true,true); 

}

void UGA_ProjectilePoke::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGA_ProjectilePoke::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{

    if(EventTag == FGameplayTag::RequestGameplayTag(FName("Event.Montage.EndAbility")))
    {
        EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
        return;
    }

    if(EventTag == FGameplayTag::RequestGameplayTag(FName("Event.Montage.SpawnProjectile")))
    {
        AMPlayerCharacter* player = Cast<AMPlayerCharacter>(GetAvatarActorFromActorInfo());

        if(!player)
        {
            EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);

        }

        //Spawn the projectile and detect collision event
        FVector Start = player->GetMesh()->GetSocketLocation(FName("RightHandSocket"));
        FVector End = player->GetCameraBoom()->GetComponentLocation() + player->GetFollowCamera()->GetForwardVector() * Range;

        FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(Start, End);

        FGameplayEffectSpecHandle DamageEffectSpecHandle = MakeOutgoingGameplayEffectSpec(DamageGameplayEffect, GetAbilityLevel());
        DamageEffectSpecHandle.Data.Get()->SetSetByCallerMagnitude(FName("Data.Damage"), Damage);
        
        FTransform SpawnTransform = player->GetMesh()->GetSocketTransform(FName("RightHandSocket"));
        SpawnTransform.SetRotation(Rotation.Quaternion());
        SpawnTransform.SetScale3D(FVector(1.0f));

        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        AMProjectile* Projectile = GetWorld()->SpawnActorDeferred<AMProjectile>(ProjectileClass, SpawnTransform, GetOwningActorFromActorInfo(),
        player, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

        Projectile->DamageEffectSpecHandle = DamageEffectSpecHandle;
        Projectile->Range = Range;
        Projectile->FinishSpawning(SpawnTransform);

    

    }


}
