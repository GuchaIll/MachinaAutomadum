
#include "MTerminal.h"
#include "HUDManager.h"

UMTerminal::UMTerminal(const FObjectInitializer& ObjectInitializer)
 : Super(ObjectInitializer)
{
    Command = "";
    History = "";
    
}


FString UMTerminal::GetCommand()
{
    return FString();
}

FString UMTerminal::ExecuteCommand()
{
    if(!Command.IsEmpty() && TerminalInterpreter.IsValid())
    return TerminalInterpreter->Interpret(Command);
    else return "Error: Missing COmmand/Interpreter";
}

void UMTerminal::UpdateText(FString text)
{
    History += "\n" + Command + "\n" + text + "\n";
    Command = "";

    return;
}

