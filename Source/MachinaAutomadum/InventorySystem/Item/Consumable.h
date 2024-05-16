// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Consumable.generated.h"

/**
 * 
 */
UCLASS()
class MACHINAAUTOMADUM_API UConsumable : public UItem
{
	GENERATED_BODY()
	
	protected:

	virtual void UseItem(class AMPlayerCharacter* Character) override;
};
