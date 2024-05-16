// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "../MGameplayAbility.h"
#include "../../Projectiles/MProjectile.h"
#include "GA_ProjectilePoke.generated.h"

/**
 * 
 */
UCLASS()
class MACHINAAUTOMADUM_API UGA_ProjectilePoke : public UMGameplayAbility
{
	GENERATED_BODY()

	public:

	UGA_ProjectilePoke();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAnimMontage* AttemptPokeMontage;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<AMProjectile> ProjectileClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<UGameplayEffect> DamageGameplayEffect;
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	protected:
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float Range;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float Damage;

	UFUNCTION()
	void OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
	void OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
	void EventReceived(FGameplayTag EventTag, FGameplayEventData EventData);

};
