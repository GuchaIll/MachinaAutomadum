// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageCalcExec.h"
#include "../Attributes/MAttributeSet.h"

struct DamageCapture
{
     DECLARE_ATTRIBUTE_CAPTUREDEF(Health);
     DECLARE_ATTRIBUTE_CAPTUREDEF(MaxHealth);

    //DELCARE_ATTRIBUTE_CAPTUREDEF(Damage); // Base Damage, Equipment Damage, Runes Damage...

    DamageCapture()
    {
        DEFINE_ATTRIBUTE_CAPTUREDEF(UMAttributeSet, Health, Source, false);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UMAttributeSet, MaxHealth, Source, false);
    }
};


static DamageCapture& GetDamageCapture()
{
    static DamageCapture Dmg;
    return Dmg;
}


UDamageCalcExec::UDamageCalcExec()
{
    RelevantAttributesToCapture.Add(GetDamageCapture().HealthDef);
    RelevantAttributesToCapture.Add(GetDamageCapture().MaxHealthDef);
    
}

void UDamageCalcExec::Execute_Implementation(const FGameplayEffectCustomExecutionParameters &ExecutionParams, FGameplayEffectCustomExecutionOutput &OutExecutionOutput) const
{
    /**Boilerplate */
    UAbilitySystemComponent* TargetAbilitySystemComponent = ExecutionParams.GetTargetAbilitySystemComponent();
    AActor* TargetActor = TargetAbilitySystemComponent ? TargetAbilitySystemComponent->GetAvatarActor() : nullptr;

    UAbilitySystemComponent* SourceAbilitySystemComponent = ExecutionParams.GetSourceAbilitySystemComponent();
    AActor* SourceActor = SourceAbilitySystemComponent ? SourceAbilitySystemComponent->GetAvatarActor() : nullptr;

    const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
    const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
    const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

    FAggregatorEvaluateParameters EvaluationParameters;
    EvaluationParameters.SourceTags = SourceTags;
    EvaluationParameters.TargetTags = TargetTags;

    /**End of Boilerplate*/
    float Health = 0.0f;
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageCapture().HealthDef, EvaluationParameters, Health);

    float MaxHealth = 0.0f;
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageCapture().MaxHealthDef, EvaluationParameters, MaxHealth);

    float HealthtoAdd = FMath::Clamp(MaxHealth-Health, 0.0f, 1.0f);

    //TargetAbilitySystemComponent->ApplyModToAttribute(DamageCapture::HealthProperty, EGameplayModOp::Additive, HealthtoAdd);
    OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(GetDamageCapture().HealthProperty,
    EGameplayModOp::Additive, HealthtoAdd));
}
