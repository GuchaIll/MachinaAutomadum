// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SaveGameData.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class ECharacterClass : uint8
{
	VE_Default UMETA(DisplayName = "Mannequin"),
	VE_Slasher UMETA (DisplayName = "Slasher"),
	VE_Sentinel UMETA (DisplayName =  "Sentinel"),
	VE_Utility UMETA (DisplayName = "Utility")
};

USTRUCT(BlueprintType)
struct FPlayerInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Info" )
	FString PlayerName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Info")
	int32 Currency;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Info")
	int32 Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Info")
	int32 Experience;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player Info")
    ECharacterClass CharacterClass;

	bool IsValid() const
	{
		return !this->PlayerName.IsEmpty() && this->Currency >= 0 && this->Level >= 0 && this->Experience >= 1;
	}

	
};

UCLASS()
class MACHINAAUTOMADUM_API USaveGameData : public USaveGame
{
	GENERATED_BODY()
	
public:

	USaveGameData();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player Info")
	FPlayerInfo SavedPlayerInfo;


};
