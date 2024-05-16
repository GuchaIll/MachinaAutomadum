#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "gameplayEffect.h"
#include "MProjectile.generated.h"

UCLASS()
class MACHINAAUTOMADUM_API AMProjectile : public AActor
{
    GENERATED_BODY()

    public:

    // Sets default values for this actor's properties
    AMProjectile();

    UPROPERTY( BlueprintReadOnly, EditAnywhere, Category = "Projectile", Meta = (ExposeOnSpawn = true))
    float Range;

    UPROPERTY( BlueprintReadOnly, EditAnywhere, Category = "GameplayEffect", Meta = (ExposeOnSpawn = true))
    FGameplayEffectSpecHandle DamageEffectSpecHandle;

    UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
    class UProjectileMovementComponent* ProjectileMovement;

protected:

    virtual void BeginPlay() override;
};