// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HUDManager.h"
#include "Interpreter.generated.h"


/**
 * 
 */
UCLASS()
class MACHINAAUTOMADUM_API AInterpreter : public AActor
{
	GENERATED_BODY()

	public:
        
        AInterpreter(const FObjectInitializer& ObjectInitializer);

        UFUNCTION(BlueprintCallable, Category = "Interpreter")
        FString Interpret(FString input);

        UPROPERTY(BlueprintReadWrite, EditAnywhere)
        TWeakObjectPtr<class UHUDManager> PlayerHUDManager;

        
};
