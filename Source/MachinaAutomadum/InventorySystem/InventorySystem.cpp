// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem.h"

// Sets default values for this component's properties
UInventorySystem::UInventorySystem()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	
	Capacity = 20;
	// ...
}


// Called when the game starts
void UInventorySystem::BeginPlay()
{
	Super::BeginPlay();

	for (auto& Item : DefaultItems)
	{
		AddItem(Item, 1);
	}
	
}



bool UInventorySystem::RemoveItem(UItem *Item, int32 Quantity)
{
	//Update to check if the item is in inventory system
	if(Item)
	{
		Item->OwningInventory = nullptr;
		Item->World = nullptr;
		Items.RemoveSingle(Item);
		OnInventoryUpdated.Broadcast();
		return true;
	}
	return false;
}

bool UInventorySystem::AddItem(UItem *Item, int32 Quantity)
{
	//Update to check if item is stackable
	//If stackable, check if item is already in inventory
	//If item is in inventory, add to stack
	//If item is not in inventory, add to inventory
	//If not stackable, add to inventory
	if(Items.Num() >= Capacity || !Item)
	{
		//Inventory is full
		return false;
	}
	Item->OwningInventory = this;
	Item->World = GetWorld();

	for(int32 i = 0; i < Quantity; i++)
	Items.Add(Item);

	OnInventoryUpdated.Broadcast();

	return true;
}

