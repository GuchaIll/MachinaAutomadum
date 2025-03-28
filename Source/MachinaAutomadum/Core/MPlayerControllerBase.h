// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MPlayerCharacter.h"
#include "MPlayerControllerBase.generated.h"

/**
 * 
 */

UCLASS()
class MACHINAAUTOMADUM_API AMPlayerControllerBase : public APlayerController
{
	GENERATED_BODY()

	public:
	
	UFUNCTION(BlueprintCallable, Category = "Character")
	void SwapCharacter(TSubclassOf<class AMPlayerCharacter> NewCharacterClass);

};
