// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageCalcExec.h"
#include "../Attributes/MAttributeSet.h"

struct DamageCapture
{
    DECLARE_ATTRIBUTE_CAPTUREDEF(BaseDamage);
    DECLARE_ATTRIBUTE_CAPTUREDEF(DamageMultiplier);
    DECLARE_ATTRIBUTE_CAPTUREDEF(Shield);
    DECLARE_ATTRIBUTE_CAPTUREDEF(MaxShield);
    DECLARE_ATTRIBUTE_CAPTUREDEF(ShieldStack);
    DECLARE_ATTRIBUTE_CAPTUREDEF(Health);
    DECLARE_ATTRIBUTE_CAPTUREDEF(MaxHealth);

    //DELCARE_ATTRIBUTE_CAPTUREDEF(Damage); // Base Damage, Equipment Damage, Runes Damage...

    DamageCapture()
    {
       
        DEFINE_ATTRIBUTE_CAPTUREDEF(UMAttributeSet, BaseDamage, Source, false);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UMAttributeSet, DamageMultiplier, Source, false);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UMAttributeSet, Shield, Target, false);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UMAttributeSet, MaxShield, Target, false);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UMAttributeSet, ShieldStack, Target, false);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UMAttributeSet, Health, Target, false);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UMAttributeSet, MaxHealth, Target, false);
    }
};


static DamageCapture& GetDamageCapture()
{
    static DamageCapture Dmg;
    return Dmg;
}


UDamageCalcExec::UDamageCalcExec()
{
    
    RelevantAttributesToCapture.Add(GetDamageCapture().BaseDamageDef);
    RelevantAttributesToCapture.Add(GetDamageCapture().DamageMultiplierDef);
    RelevantAttributesToCapture.Add(GetDamageCapture().ShieldDef);
    RelevantAttributesToCapture.Add(GetDamageCapture().MaxShieldDef);
    RelevantAttributesToCapture.Add(GetDamageCapture().ShieldStackDef);
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
    float BaseDamageValue = 0.0f;
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageCapture().BaseDamageDef, EvaluationParameters, BaseDamageValue);
    BaseDamageValue += FMath::Max<float>(Spec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Damage")), false, -1.0f), 0.0f);
    
    float DamageMultiplierValue = 0.0f;
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageCapture().DamageMultiplierDef, EvaluationParameters, DamageMultiplierValue);

    float ShieldValue = 0.0f;
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageCapture().ShieldDef, EvaluationParameters, ShieldValue);

    float MaxShieldValue = 0.0f;
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageCapture().MaxShieldDef, EvaluationParameters, MaxShieldValue);

    float ShieldStackValue = 0.0f;
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageCapture().ShieldStackDef, EvaluationParameters, ShieldStackValue);

    float CurrentShieldValue = ShieldStackValue * MaxShieldValue + ShieldValue;

    float UnmitigatedDamage = BaseDamageValue * DamageMultiplierValue;

    float HealthValue = 0.0f;
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageCapture().HealthDef, EvaluationParameters, HealthValue);

    float MaxHealthValue = 0.0f;
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageCapture().MaxHealthDef, EvaluationParameters, MaxHealthValue);

    //Calculate Mitigated Damage After Shield
    if(CurrentShieldValue >= UnmitigatedDamage)
    {
        CurrentShieldValue -= UnmitigatedDamage;
        float CurrentShieldStack = CurrentShieldValue / MaxShieldValue;
        float RemainingShield = CurrentShieldValue - (CurrentShieldStack * MaxShieldValue);
        OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(GetDamageCapture().ShieldProperty, EGameplayModOp::Override, RemainingShield));
        OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(GetDamageCapture().ShieldStackProperty, EGameplayModOp::Override, CurrentShieldStack));
    }
    else
    {
        float MitigatedDamage = UnmitigatedDamage - CurrentShieldValue;
        MitigatedDamage = FMath::Min<float>(MitigatedDamage, HealthValue);
        OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(GetDamageCapture().ShieldProperty,  EGameplayModOp::Override, 0.0f));
        OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(GetDamageCapture().ShieldStackProperty, EGameplayModOp::Override, 0.0f));
        OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(GetDamageCapture().HealthProperty, EGameplayModOp::Additive, -MitigatedDamage));
        
    }   

    //Broadcast Damage to All Enemy Targets Hit

   
}
