// Fill out your copyright notice in the Description page of Project Settings.


#include "Consumable.h"
#include "../InventorySystem.h"

 void UConsumable::UseItem(class AMPlayerCharacter* Character)
 {
    if(ItemStackSize > 1)
    {
        ItemStackSize--;
    }
    else
    {
        OwningInventory->RemoveItem(this, 1);
    }
    
 }