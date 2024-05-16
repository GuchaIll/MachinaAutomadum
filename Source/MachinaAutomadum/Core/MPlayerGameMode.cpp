// Fill out your copyright notice in the Description page of Project Settings.


#include "MPlayerGameMode.h"
#include "MPlayerCharacter.h"
#include "MGameInstance.h"
#include "../SavingSystem/SaveGameData.h"
#include "../Characters/MSentinelCharacter.h"
#include "../Characters/MSlasherCharacter.h"
#include "../Characters/MUtilityCharacter.h"

AMPlayerGameMode::AMPlayerGameMode()
{

}

void AMPlayerGameMode::UpdatePlayerStartingPoint(FVector NewStartPoint)
{
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    ACharacter* NewCharacter = GetWorld()->SpawnActor<AMPlayerCharacter>(AMPlayerCharacter::StaticClass(), NewStartPoint, FRotator::ZeroRotator, SpawnParams);

    if (NewCharacter)
    {
        // If there is a player controller, possess the newly spawned character
        APlayerController* PC = GetWorld()->GetFirstPlayerController();
        if (PC)
        {
            PC->Possess(NewCharacter);
        }
    }
}

AMPlayerCharacter* AMPlayerGameMode::DetermineCharacterClass( FVector TransFormLocation)
{
    UClass* CharacterClassToSpawn;

    UMGameInstance* MyGameInstance = Cast<UMGameInstance>(GetWorld()->GetGameInstance());
    
    ECharacterClass CurrentCharacterType = (MyGameInstance == nullptr) ? ECharacterClass::VE_Default : MyGameInstance->CharacterClass;

    switch (CurrentCharacterType)
    {
        case ECharacterClass::VE_Slasher:
            CharacterClassToSpawn = AMSlasherCharacter::StaticClass();
            break;
        case ECharacterClass::VE_Sentinel:
            CharacterClassToSpawn = AMSentinelCharacter::StaticClass();
            break;
        case ECharacterClass::VE_Utility:
            CharacterClassToSpawn = AMUtilityCharacter::StaticClass();
            break;

        default:
            CharacterClassToSpawn = AMPlayerCharacter::StaticClass();
            break;
    }

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    AMPlayerCharacter* NewCharacter = GetWorld()->SpawnActor<AMPlayerCharacter>(CharacterClassToSpawn, TransFormLocation, FRotator::ZeroRotator, SpawnParams);

    if (NewCharacter)
    {
        APlayerController* PC = GetWorld()->GetFirstPlayerController();
        if (PC)
        {
            PC->Possess(NewCharacter);
        }
    }

    return NewCharacter;       
    }
  