
#include "MTerminal.h"
#include "HUDManager.h"
#include "Interpreter.h"

UMTerminal::UMTerminal(const FObjectInitializer& ObjectInitializer)
 : Super(ObjectInitializer)
{
    Command = "";
    
    AInterpreter* InterpreterInstance = nullptr;
    if(GetWorld() != nullptr) InterpreterInstance = GetWorld()->SpawnActor<AInterpreter>(AInterpreter::StaticClass());
    if (InterpreterInstance)
    {
    // Assuming you have access to an instance of MTerminal here
       
        TerminalInterpreter = InterpreterInstance;
    } 
}


FString UMTerminal::GetCommand()
{
    return FString();
}

FString UMTerminal::ExecuteCommand()
{
    if(!Command.IsEmpty() && TerminalInterpreter.IsValid())
    return TerminalInterpreter->Interpret(Command);
    else return "Error: Missing Command/Interpreter";
}

void UMTerminal::UpdateText(FString text)
{
   
    Command = "";

    return;
}

