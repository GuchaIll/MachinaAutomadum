// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "DamageCalcExec.generated.h"

/**
 * 
 */
UCLASS()
class MACHINAAUTOMADUM_API UDamageCalcExec : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

	UDamageCalcExec();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, 
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
	


	
};
