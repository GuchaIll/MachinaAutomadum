#include "EnergyExecCalc.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"
#include "../Attributes/MAttributeSet.h"

struct EnergyCapture
{
    DECLARE_ATTRIBUTE_CAPTUREDEF(Energy);
    DECLARE_ATTRIBUTE_CAPTUREDEF(MaxEnergy);

    EnergyCapture()
    {
        DEFINE_ATTRIBUTE_CAPTUREDEF(UMAttributeSet, Energy, Target, false);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UMAttributeSet, MaxEnergy, Target, false);
    }
};

static const EnergyCapture& GetEnergyCapture()
{
    static EnergyCapture Capture;
    return Capture;
}

UEnergyExecCalc::UEnergyExecCalc()
{
    RelevantAttributesToCapture.Add(GetEnergyCapture().EnergyDef);
    RelevantAttributesToCapture.Add(GetEnergyCapture().MaxEnergyDef);
}

void UEnergyExecCalc::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
    UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
    if (!TargetASC)
    {
        return;
    }

    const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

    // Capture the current energy value
    float Energy = 0.0f;
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetEnergyCapture().EnergyDef, FAggregatorEvaluateParameters(), Energy);

    float MaxEnergy = 0.0f;
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetEnergyCapture().MaxEnergyDef, FAggregatorEvaluateParameters(), MaxEnergy);

    // Calculate the energy cost (this could be based on the spec or other logic)
    float EnergyCost = Spec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.EnergyCost")), false, -1.0f);

    if (EnergyCost > 0.0f && Energy >= EnergyCost)
    {
        // Apply the energy cost
        OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(GetEnergyCapture().EnergyProperty, EGameplayModOp::Additive, -EnergyCost));
    }
    else if(EnergyCost < 0.0f)
    {
        EnergyCost = FMath::Abs(EnergyCost);
        EnergyCost = FMath::Clamp( Energy + EnergyCost, 0.0f, MaxEnergy);
        // Apply the energy cost
        OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(GetEnergyCapture().EnergyProperty, EGameplayModOp::Override, EnergyCost));
    }
}