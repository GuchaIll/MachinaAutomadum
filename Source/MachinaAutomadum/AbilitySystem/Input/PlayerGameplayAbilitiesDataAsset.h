// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Abilities/GameplayAbility.h" //Require implementation
#include "PlayerGameplayAbilitiesDataAsset.generated.h"

/**
 * 
 */

class UInputAction;

USTRUCT()
struct FGameplayInputAbilityInfo
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditAnywhere, Category = "GameplayInputAbilityInfo" )
	TSubclassOf<UGameplayAbility> GameplayAbilityClass;

	UPROPERTY(EditAnywhere, Category = "GameplayInputAbilityInfo")
	TObjectPtr<UInputAction> InputAction;

	UPROPERTY(VisibleAnywhere, Category = "GameplayInputAbilityInfo")
	int32 InputID;

	bool IsValid() const
	{
		return GameplayAbilityClass && InputAction;
	}

	bool operator==(const FGameplayInputAbilityInfo& Other) const
	{
		return GameplayAbilityClass == Other.GameplayAbilityClass && InputAction == Other.InputAction;
	}
 	bool operator!=(const FGameplayInputAbilityInfo& Other) const
 	{
  		return !operator==(Other);
	 }

	friend uint32 GetTypeHash(const FGameplayInputAbilityInfo& Item)
	{
		return HashCombine(GetTypeHash(Item.GameplayAbilityClass), GetTypeHash(Item.InputAction));
	}
};

UCLASS()
class MACHINAAUTOMADUM_API UPlayerGameplayAbilitiesDataAsset : public UDataAsset
{
	GENERATED_BODY()

	protected:

	UPROPERTY(EditAnywhere, Category = "GameplayAbilities")
	TSet<FGameplayInputAbilityInfo> InputAbilities;

	public:

	UPlayerGameplayAbilitiesDataAsset(const FObjectInitializer& ObjectInitializer);
	
	const TSet<FGameplayInputAbilityInfo> & GetInputAbilities() const;

	#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	#endif
	
};
