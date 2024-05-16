// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"


#include "Item.generated.h"

/**
 * 
 */

class UInventorySystem;
class AMPlayerCharacter;

UCLASS(Abstract, BlueprintType, Blueprintable, EditInlineNew, DefaultToInstanced)
class MACHINAAUTOMADUM_API UItem : public UObject
{
	GENERATED_BODY()

	public:

	UItem();

	virtual class UWorld* GetWorld() const { return World; }
	UPROPERTY(Transient)
	class UWorld* World;

	/**Text for using the item*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
	FText UseActionText;
	
	/**Mesh Displayed for item pick up*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
	class UStaticMesh* PickupMesh;

	/**Thumbnail for Item*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
	class UTexture2D* Thumbnail;

	/**Item Display Name*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
	FText ItemDisplayName;

	/**Item Description*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item", meta = (MultiLine = true))
	FText ItemDescription;

	/**Is Stackable?*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
	bool bStackable;

	/**Item Stack Size*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item", meta = (EditCondition = "bStackable"))
	int32 ItemStackSize;

	/**Max Stack Size*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item", meta = (EditCondition = "bStackable"))
	int32 MaxStackSize;

	/**The inventory the item is contained in*/
	UPROPERTY()
	class UInventorySystem* OwningInventory;

	virtual void UseItem(class AMPlayerCharacter* Character) PURE_VIRTUAL(UItem::UseItem, );

	UFUNCTION(BlueprintImplementableEvent)
	void OnUseItem(class AMPlayerCharacter* Character);

};
