// Fill out your copyright notice in the Description page of Project Settings.

#include "MGameInstance.h"
#include "../SavingSystem/SaveGameData.h"
#include "Kismet/GameplayStatics.h"

UMGameInstance::UMGameInstance()
{
    OnSaveGame.AddDynamic(this, &UMGameInstance::SaveGame);
}

void UMGameInstance::SaveGame()
{
    SaveGameInstance = Cast<USaveGameData>(UGameplayStatics::CreateSaveGameObject(USaveGameData::StaticClass()));

    if (SaveGameInstance != nullptr)
    {
        SaveGameInstance->SavedPlayerInfo = CurrentPlayerInfo;

        UGameplayStatics::AsyncSaveGameToSlot(SaveGameInstance, TEXT("DefaultSlot"), 0,
                                               FAsyncSaveGameToSlotDelegate::CreateLambda([this](const FString &SlotName,  int32 UserIndex, bool bSuccess)
                                                                                          {

            if(bSuccess)
            {
                UE_LOG(LogTemp, Warning, TEXT("Game saved successfullly!"));
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Failed to save game!"));
            } }));
    }
}

void UMGameInstance::LoadGame()
{
    FAsyncLoadGameFromSlotDelegate LoadGameDelegate = FAsyncLoadGameFromSlotDelegate::CreateLambda([this](const FString &SlotName, int32 UserIndex, USaveGame *LoadedData)
                
     {
         USaveGameData* LoadedGameData = Cast<USaveGameData>(LoadedData);
        if(LoadedGameData != nullptr)
        {
            CurrentPlayerInfo = LoadedGameData->SavedPlayerInfo;
            
            OnLoadGame.Broadcast();

            if(CurrentPlayerInfo.IsValid())
            {
                UE_LOG(LogTemp, Warning, TEXT("Game loaded successfully!"));
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Failed to load game!"));
            }
        }
    });
}
