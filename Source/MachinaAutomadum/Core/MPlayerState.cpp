// Fill out your copyright notice in the Description page of Project Settings.


#include "MPlayerState.h"
#include "MPlayerCharacter.h"

// Sets default values for this component's properties
UMPlayerState::UMPlayerState()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	//PrimaryComponentTick.bCanEverTick = true;

	// ...
	
}

void UMPlayerState::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {}

void UMPlayerState::BeginPlay() {}
void UMPlayerState::SwapCharacter(TSubclassOf<class AMPlayerCharacter> NewCharacterClass)
{
	FVector Location = CurrentCharacter->GetActorLocation();
	FRotator Rotation = CurrentCharacter->GetActorRotation();

	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	AMPlayerCharacter* NewCharacter = GetWorld()->SpawnActor<AMPlayerCharacter>(NewCharacterClass, Location, Rotation, SpawnInfo);
	
	CurrentCharacter = NewCharacter;
}

