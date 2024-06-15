// Fill out your copyright notice in the Description page of Project Settings.


#include "MPlayerControllerBase.h"
#include "MPlayerCharacter.h"

void AMPlayerControllerBase::SwapCharacter(TSubclassOf<class AMPlayerCharacter> NewCharacterClass)
{
    AMPlayerCharacter* CurrentCharacter = Cast<AMPlayerCharacter>(GetPawn());

    if(CurrentCharacter != nullptr)
    {
        FVector Location = CurrentCharacter->GetActorLocation();
        FRotator Rotation = CurrentCharacter->GetActorRotation();

        FActorSpawnParameters SpawnInfo;
        SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
        AMPlayerCharacter* NewCharacter = GetWorld()->SpawnActor<AMPlayerCharacter>(NewCharacterClass, Location, Rotation, SpawnInfo);

              // Destroy the old character
         

        if (NewCharacter != nullptr)
        {
            // Unpossess the old character
            UnPossess();

            // Possess the new character
            Possess(NewCharacter);

             // Set up the input component for the new character
            NewCharacter->SetupPlayerInputComponent(InputComponent);

            CurrentCharacter->Destroy();

        
        }
    }
}