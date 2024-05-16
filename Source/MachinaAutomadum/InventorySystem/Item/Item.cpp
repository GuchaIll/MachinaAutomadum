// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"

UItem::UItem() {

    ItemDisplayName = FText::FromString("Item");
    ItemDescription = FText::FromString("Item Description");
    bStackable = false;
    ItemStackSize = 1;
}

