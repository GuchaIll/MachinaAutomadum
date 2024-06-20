
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDManager.generated.h"


/**
 * 
 */

class UMFloatingStatusBar;
class UMTerminal;
class PlayerHUDWidget;

UCLASS()
class MACHINAAUTOMADUM_API UHUDManager : public UActorComponent
{
	GENERATED_BODY()

	public:

        //Push Widget onto stack
		UFUNCTION(BlueprintCallable)
        void Push(UUserWidget* widget);

        //Pop widget from the stack
		UFUNCTION(BlueprintCallable)
        UUserWidget* Pop();

        //Peek at the top widget on the stack
        UFUNCTION(BlueprintCallable)
        UUserWidget* Peek() const;
        

	private:

        //Stack of widgets
        UPROPERTY()
        TArray<UUserWidget*> WidgetStack;
	
};

