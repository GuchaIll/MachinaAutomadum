#include "GE_EnergyCost.h"
#include "../Attributes/MAttributeSet.h"
#include "../ExecutionClass/EnergyExecCalc.h"
#include "GameplayTagContainer.h"

UGE_EnergyCost::UGE_EnergyCost()
{
    // Set the duration policy to instant
    DurationPolicy = EGameplayEffectDurationType::Instant;

    // Set the execution calculation class
    FGameplayEffectExecutionDefinition ExecutionDefinition;
    ExecutionDefinition.CalculationClass = UEnergyExecCalc::StaticClass();
    Executions.Add(ExecutionDefinition);

    // Add a SetByCaller tag for the energy cost
    FGameplayModifierInfo ModifierInfo;
    ModifierInfo.Attribute = FGameplayAttribute(FindFieldChecked<FProperty>(UMAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UMAttributeSet, Energy)));
    ModifierInfo.ModifierOp = EGameplayModOp::Additive;
    ModifierInfo.ModifierMagnitude = FScalableFloat(0.0f); // Default value, will be set by caller

    // Use FGameplayTagContainer to add tags
    ModifierInfo.SourceTags.RequireTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Data.EnergyCost")));
    Modifiers.Add(ModifierInfo);
}