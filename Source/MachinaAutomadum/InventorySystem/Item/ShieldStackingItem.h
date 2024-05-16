#pragma once

#include "CoreMinimal.h"
#include "Consumable.h"
#include "ShieldStackingItem.generated.h"

UCLASS()
class MACHINAAUTOMADUM_API UShieldStackingItem : public UConsumable
{
	GENERATED_BODY()
	
	protected:

	virtual void UseItem(class AMPlayerCharacter* Character) override;
    
};
