
#include "MTerminal.h"
#include "HUDManager.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Interpreter.h"

UMTerminal::UMTerminal(const FObjectInitializer& ObjectInitializer)
 : Super(ObjectInitializer)
{
    Command = "";
    
   if (GetWorld() != nullptr)
    {
        UClass* InterpreterClass = AInterpreter::StaticClass();
        if (InterpreterClass != nullptr)
        {
            FActorSpawnParameters SpawnParams;
            SpawnParams.Owner = GetOwningPlayerPawn();
            

            // Log before spawning
            UE_LOG(LogTemp, Log, TEXT("Spawning Interpreter"));

            // Use SpawnActorDeferred for safer spawning
            AInterpreter* DeferredInterpreter = GetWorld()->SpawnActorDeferred<AInterpreter>(InterpreterClass, FTransform::Identity, SpawnParams.Owner, nullptr);
            if (DeferredInterpreter)
            {
                // Initialize any properties on DeferredInterpreter here if needed

                // Finish spawning
                DeferredInterpreter->FinishSpawning(FTransform::Identity);
                InterpreterInstance = DeferredInterpreter;

                // Log after spawning
                UE_LOG(LogTemp, Log, TEXT("Interpreter spawned successfully"));
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("Failed to spawn Interpreter"));
            }
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("InterpreterClass is null"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("GetWorld() returned null"));
    }
}


FString UMTerminal::GetCommand()
{
    return FString();
}

FString UMTerminal::ExecuteCommand()
{
    if(!Command.IsEmpty() && InterpreterInstance.IsValid())
    return InterpreterInstance->Interpret(Command);
    else return "Error: Missing Command/Interpreter";
}

void UMTerminal::UpdateText(FString text)
{
   
    Command = "";

    return;
}

