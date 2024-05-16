// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../MGameplayAbility.h"
#include "GA_Dash.generated.h"

/**
 * 
 */
UCLASS()
class MACHINAAUTOMADUM_API UGA_Dash : public UMGameplayAbility
{
	GENERATED_BODY()
	
	public:

	UGA_Dash();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	void SetCooldown();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	float DashDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	float DashSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	float DashCooldown;


};
