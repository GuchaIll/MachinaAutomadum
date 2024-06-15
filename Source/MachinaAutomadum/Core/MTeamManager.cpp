// Fill out your copyright notice in the Description page of Project Settings.


#include "MTeamManager.h"
#include "MPlayerCharacter.h"

// Sets default values for this component's properties
UMTeamManager::UMTeamManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	//PrimaryComponentTick.bCanEverTick = true;

	// ...
	
}

void UMTeamManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {}

void UMTeamManager::BeginPlay() {}

void UMTeamManager::SwapCharacter(TSubclassOf<class AMPlayerCharacter> NewCharacterClass)
{
	if(CurrentCharacter != nullptr)
	{
		FVector Location = CurrentCharacter->GetActorLocation();
		FRotator Rotation = CurrentCharacter->GetActorRotation();

		FActorSpawnParameters SpawnInfo;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		AMPlayerCharacter* NewCharacter = GetWorld()->SpawnActor<AMPlayerCharacter>(NewCharacterClass, Location, Rotation, SpawnInfo);

		if(NewCharacter != nullptr)
		{
			CurrentCharacter->Destroy();
			CurrentCharacter = NewCharacter;
			APlayerController* PC = GetWorld()->GetPlayerControllerIterator()->Get();

			if(PC != nullptr)
			{
				PC->Possess(CurrentCharacter);
			}
			else{
				if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Player Controller is null"));
			}
		} 
		else
		{
			if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("New Character is null"));
		}
		
	}
	else{
		if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Current Character is null"));
	}

}

