// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "MAbilitySystemComponent.generated.h"

class AActor;
class UGameplayAbility;
class ULyraAbilityTagRelationshipMapping;
class UObject;
struct FFrame;
struct FGameplayAbilityTargetDataHandle;

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FReceivedDamageDelegate, UMAbilitySystemComponent*, SourceASC, float, UnmitigatedDamage, float, MitigatedDamage);
UCLASS()
class MACHINAAUTOMADUM_API UMAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	public:

	UMAbilitySystemComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	bool bCharacterAbilitiesGiven = false;
	bool bStartupEffectsApplied = false;

	FReceivedDamageDelegate ReceivedDamageDelegate;

	virtual void ReceiveDamage(AActor* SourceActor, float UnmitigatedDamage, float MitigatedDamage);
};
