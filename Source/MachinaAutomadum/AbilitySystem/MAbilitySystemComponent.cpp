// Fill out your copyright notice in the Description page of Project Settings.


#include "MAbilitySystemComponent.h"


UMAbilitySystemComponent::UMAbilitySystemComponent(const FObjectInitializer &ObjectInitializer)
: Super(ObjectInitializer)
{
    IsWieldingWeapon = false;
    ComboHit = 0;

    AActor* OwnedActor = GetOwner();
    if (OwnedActor)
    {
        // Attempt to get the Skeletal Mesh Component from the owner
        USkeletalMeshComponent* SkeletalMeshComp = OwnedActor->FindComponentByClass<USkeletalMeshComponent>();
        if (SkeletalMeshComp)
        {
            // Get the AnimInstance from the Skeletal Mesh Component
            AnimInstance = SkeletalMeshComp->GetAnimInstance();
        }
    }

}

void UMAbilitySystemComponent::ReceiveDamage(AActor *SourceActor, float UnmitigatedDamage, float MitigatedDamage)
{
    ReceivedDamageDelegate.Broadcast(this, UnmitigatedDamage, MitigatedDamage);
}
