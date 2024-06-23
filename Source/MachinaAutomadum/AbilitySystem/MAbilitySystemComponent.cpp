// Fill out your copyright notice in the Description page of Project Settings.


#include "MAbilitySystemComponent.h"


UMAbilitySystemComponent::UMAbilitySystemComponent(const FObjectInitializer &ObjectInitializer)
: Super(ObjectInitializer)
{
    IsWieldingWeapon = false;
}

void UMAbilitySystemComponent::ReceiveDamage(AActor *SourceActor, float UnmitigatedDamage, float MitigatedDamage)
{
    ReceivedDamageDelegate.Broadcast(this, UnmitigatedDamage, MitigatedDamage);
}
