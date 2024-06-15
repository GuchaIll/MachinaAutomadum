// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MFloatingStatusBar.generated.h"

/**
 * 
 */
UCLASS()
class MACHINAAUTOMADUM_API UMFloatingStatusBar : public UUserWidget
{
	GENERATED_BODY()

	public:

		UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void SetHealth(float newHealth, float newMaxHealth);

		UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void SetArmor(float newArmor, float newMaxArmor);

		UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void SetEnergy(float newEnergy, float newMaxEnergy);

		UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void SetLevel(int newLevel);


	
	
};
