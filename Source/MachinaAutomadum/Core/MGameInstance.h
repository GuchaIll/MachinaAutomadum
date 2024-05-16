// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "../SavingSystem/SaveGameData.h"

#include "MGameInstance.generated.h"


/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSaveGameDelegate);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLoadGameDelegate);


UCLASS()
class MACHINAAUTOMADUM_API UMGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
	public:

	UMGameInstance();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player References")
	ECharacterClass CharacterClass;

	UPROPERTY(BlueprintReadOnly, Category = "Save_Info" )
	USaveGameData* SaveGameInstance;

	UPROPERTY(BlueprintAssignable, Category = "Events")
    FSaveGameDelegate OnSaveGame;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FLoadGameDelegate OnLoadGame;

	UPROPERTY(BlueprintReadWrite, Category = "Save_Info")
	FPlayerInfo CurrentPlayerInfo;

	UFUNCTION(BlueprintCallable, Category = "Save_System")
	void SaveGame();

	UFUNCTION(BlueprintCallable, Category = "Save_System")
	void LoadGame();
	
};
