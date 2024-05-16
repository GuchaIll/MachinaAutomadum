// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "../MAbilitySystemComponent.h"
#include "GAT_WaitReceiveDamage.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FWaitReceiveDamageDelegate, class UMAbilitySystemComponent*, SourceASC, float, UnmitigatedDamage, float, MitigatedDamage);

UCLASS()
class MACHINAAUTOMADUM_API UGAT_WaitReceiveDamage : public UAbilityTask
{
	GENERATED_BODY()

	public:

	UGAT_WaitReceiveDamage(const FObjectInitializer& ObjectInitializer);

	//UPROPERTY(BlueprintAssignable)
	//FWaitReceiveDamageDelegate OnDamage;

	//virtual void Activate() override;

	//virtual void OnGameplayEvent(FGameplayTag EventTag, const FGameplayEventData* EventData);

	//virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);

	//bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) const;

	//UFUNCTION()
	//void SetSourceActor(AActor* SourceActor);

	//UFUNCTION()
	//void OnDamageReceived(class UMAbilitySystemComponent* SourceASC, float UnmitigatedDamage, float MitigatedDamage);

	//UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	//static UGAT_WaitReceiveDamage* WaitReceiveDamage(UGameplayAbility* OwningAbility, bool TriggerOnce);

	
//protected:
	bool TriggerOnce;

	//AActor* SourceActor = nullptr;

	//virtual void OnDestroy(bool AbilityIsEnding) override;
};
