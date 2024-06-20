#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interpreter.h"
#include "MTerminal.generated.h"


UCLASS()
class MACHINAAUTOMADUM_API UMTerminal : public UUserWidget
{
	GENERATED_BODY()



	public:

        UMTerminal(const FObjectInitializer& ObjectInitializer);

		UFUNCTION(BlueprintCallable)
		FString GetCommand();

        UFUNCTION(BlueprintCallable)
        FString ExecuteCommand();

        UFUNCTION(BlueprintCallable)
        void UpdateText(FString text);

        UPROPERTY(BlueprintReadWrite, EditAnywhere)
        FString Command;

        UPROPERTY(BlueprintReadWrite, EditAnywhere)
        FString History;

        UPROPERTY(BlueprintReadWrite, EditAnywhere)
        TWeakObjectPtr<class AInterpreter> TerminalInterpreter;
	
};