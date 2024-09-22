// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "./MPlayerCharacter.h"
#include "MGameInstance.h"
#include "MPlayerGameMode.generated.h"

/**
 * 
 */
class AMPlayerCharacter;



UCLASS()
class MACHINAAUTOMADUM_API AMPlayerGameMode : public AGameModeBase
{
	GENERATED_BODY()

	public:

	AMPlayerGameMode();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Reference")
	AMPlayerCharacter* Player;

	



};
