// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MTeamManager.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MACHINAAUTOMADUM_API UMTeamManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMTeamManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Swap_Character")
	void SwapCharacter(TSubclassOf<class AMPlayerCharacter> NewCharacterClass);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Selected_Character", meta = (AllowPrivateAccess = "true"))
   AMPlayerCharacter* CurrentCharacter;
   
protected: 
  

   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Selected_Character")
   TSubclassOf<class AMPlayerCharacter> PrimaryCharacter;

   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Selected_Character")
   TSubclassOf<class AMPlayerCharacter> SecondaryCharacter;

   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Selected_Character")
   TSubclassOf<class AMPlayerCharacter> TertiaryCharacter;




   //Equipment classes for the current character

   //Custom Loadout: Memory Fragment, Weapon, Armor, Runes, Ability Upgrades, Parts Upgrades





		
};
