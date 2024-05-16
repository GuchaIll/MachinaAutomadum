
#include "MProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"


// Sets default values
AMProjectile::AMProjectile()
{
    PrimaryActorTick.bCanEverTick = true;
    bReplicates = true;
    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
}

void AMProjectile::BeginPlay()
{
    Super::BeginPlay();
}