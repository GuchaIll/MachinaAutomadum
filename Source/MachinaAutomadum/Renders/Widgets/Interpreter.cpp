
#include "Interpreter.h"
#include "HUDManager.h"

AInterpreter::AInterpreter(const FObjectInitializer& ObjectInitializer)
{
    UWorld* World = GetWorld();
    if(World)
    {
          APlayerController* PlayerController = World->GetFirstPlayerController();
            if(PlayerController)
            {
                 PlayerHUDManager = Cast<UHUDManager>(PlayerController->GetComponentByClass(UHUDManager::StaticClass()));

            }
    }
   
}

FString AInterpreter::Interpret(FString input)
{
      if(input.IsEmpty())
    {
        return FString("No input entered.");
    }
    else if(input == "help")
    {
        return FString("Available inputs: help, clear, exit, ls, cd, cat, rm, mv, cp, mkdir, rmdir, touch");
    }
    else if(input == "arsenal")
    {
        return FString("Previewing arsenal...");
    }
    else if(input == "mission")
    {
        
        return FString("Opening mission terminal...");
    }
    else if(input == "inventory")
    {
        
        //PlayerHUDManager->Push(InventoryWidget);
        return FString("Opening inventory...");
    }
    else if(input == "memory")
    {   
        //PlayerHUDManager->Push(MemoryWidget);
        return FString("Stimulating memory system...");
    } 
    else if(input == "team")
    {
       // PlayerPlayerHUDManager->Push(TeamWidget);
        return FString ("Opening team terminal...");
    }  
    else if(input == "character")
    {
       // PlayerPlayerHUDManager->Push(CharacterWidget);
        return FString("Opening character terminal...");
    }
    else if(input == "level")
    {
        //PlayerPlayerHUDManager->Push(LevelWidget);
        return FString("choosing level...");
    }
    else if(input == "map")
    {
       // PlayerPlayerHUDManager->Push(MapWidget);
        return FString("Level map...");
    }
    else if(input == "setting")
    {
       // PlayerPlayerHUDManager->Push(SettingWidget);
        return FString("Opening settings...");
    }
    else if(input == "about")
    {
        return FString("About Machina Automadum...");
    }
    else if(input == "clear")
    {
        
        return FString();
    }
    else if(input == "exit")
    {
       // if(PlayerPlayerHUDManager.IsValid())PlayerPlayerHUDManager->Pop();
       // else return FString("PlayerPlayerHUDManager is not valid.");

        return FString("Exiting terminal...");
    }
    else if(input == "ls")
    {
        //Entity memory system
        //Listing all files in the current directory
        return FString("Listing files...");
    }
    else if(input == "cd")
    {
        //Entity memory system
        return FString("Changing directory...");
    }
    else if(input == "cat")
    {

        return FString("Displaying file contents...");
    }
    else if(input == "rm")
    {
        return FString("Removing file...");
    }
    else if(input == "mv")
    {
        return FString("Moving file...");
    }
    else if(input == "cp")
    {
        return FString("Copying file...");
    }
    else
    {
        return FString("input not found.");
    }
}