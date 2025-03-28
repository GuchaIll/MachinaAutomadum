// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerGameplayAbilitiesDataAsset.h"
#include "../Abilities/MGameplayAbility.h"

UPlayerGameplayAbilitiesDataAsset::UPlayerGameplayAbilitiesDataAsset(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
}

const TSet<FGameplayInputAbilityInfo>& UPlayerGameplayAbilitiesDataAsset::GetInputAbilities() const
{
    return InputAbilities;
}

#if WITH_EDITOR
void UPlayerGameplayAbilitiesDataAsset::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);

    const FProperty* Property = PropertyChangedEvent.Property;
    if (Property && Property->GetName() == GET_MEMBER_NAME_CHECKED(UPlayerGameplayAbilitiesDataAsset, InputAbilities) && !InputAbilities.IsEmpty())
    {
      TArray<FGameplayInputAbilityInfo> InputAbilitiesArray = InputAbilities.Array();
      InputAbilities.Reset();

      for(int32 i = 0; i < InputAbilitiesArray.Num(); ++i)
      {
        FGameplayInputAbilityInfo& Info = InputAbilitiesArray[i];
        Info.InputID = i;
        InputAbilities.Add(Info);
      }
    }
}
#endif