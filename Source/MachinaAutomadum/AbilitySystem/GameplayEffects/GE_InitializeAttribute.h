// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "GE_InitializeAttribute.generated.h"

/**
 * 
 */
UCLASS()
class MACHINAAUTOMADUM_API UGE_InitializeAttribute : public UGameplayEffect
{
	GENERATED_BODY()
	
	public:

	UPROPERTY(EditDefaultsOnly, Category = "Attribute")
	UDataTable* AttributeDataTable;

	virtual void PostInitProperties() override;
};
