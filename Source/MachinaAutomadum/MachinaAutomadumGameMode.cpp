// Copyright Epic Games, Inc. All Rights Reserved.

#include "MachinaAutomadumGameMode.h"
#include "MachinaAutomadumCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMachinaAutomadumGameMode::AMachinaAutomadumGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
