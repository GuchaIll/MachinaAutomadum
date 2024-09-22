#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "EnergyExecCalc.generated.h"

UCLASS()
class MACHINAAUTOMADUM_API UEnergyExecCalc : public UGameplayEffectExecutionCalculation
{
    GENERATED_BODY()

public:
    UEnergyExecCalc();

    virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};