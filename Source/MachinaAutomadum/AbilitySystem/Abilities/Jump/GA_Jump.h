// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../MGameplayAbility.h"
#include "GA_Jump.generated.h"

/**
 * 
 */
UCLASS()
class MACHINAAUTOMADUM_API UGA_Jump : public UMGameplayAbility
{
	GENERATED_BODY()
	
	public:

	UGA_Jump();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	void ResetTimer();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	float JumpDistance;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
    float JumpHeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	float JumpCooldown;

	UPROPERTY()
	FTimerHandle JumpTimerHandle;


};