// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class MACHINAAUTOMADUM_API UPlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()

	public:
		UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void SetHealth(float Health, float MaxHealth);

		UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void SetArmor(float Armor, float MaxArmor);

		UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void SetEnergy(float Energy, float MaxEnergy);

		UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void SetLevel(int Level);

		UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void SetExperience(float Experience, float MaxExperience);

		
	
};
