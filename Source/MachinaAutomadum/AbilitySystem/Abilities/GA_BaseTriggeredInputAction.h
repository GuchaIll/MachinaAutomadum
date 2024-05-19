// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MGameplayAbility.h"
#include "GA_BaseTriggeredInputAction.generated.h"

/**
 * 
 */

struct FInputActionValue;

UCLASS()
class MACHINAAUTOMADUM_API UGA_BaseTriggeredInputAction : public UMGameplayAbility
{
	GENERATED_BODY()

	public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	bool bCancelAbilityOnInputRelease;


	UGA_BaseTriggeredInputAction(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	

	UPROPERTY()
	TObjectPtr<UEnhancedInputComponent> EnhancedInputComponent;
	
	TArray<uint32> TriggeredEventHandles;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

	virtual void OnTriggeredInputAction (const FInputActionValue& Value);

};
