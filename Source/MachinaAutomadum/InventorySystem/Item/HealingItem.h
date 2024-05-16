#pragma once

#include "CoreMinimal.h"
#include "Consumable.h"
#include "HealingItem.generated.h"

UCLASS()
class MACHINAAUTOMADUM_API UHealingItem : public UConsumable
{
	GENERATED_BODY()
	
	protected:

	virtual void UseItem(class AMPlayerCharacter* Character) override;

};
