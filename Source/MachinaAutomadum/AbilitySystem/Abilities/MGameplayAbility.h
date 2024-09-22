// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "MGameplayAbility.generated.h"

/**
 * 
 */
class AActor;
class AController;
class APlayerController;


UENUM(BlueprintType)
enum class EMAbilityInputID : uint8
{
	None,
	Confirm,
	Cancel,
	Ability1,
	Ability2,
	Ultimate,
	ResetCamera
};

UENUM(BlueprintType)
enum class EMAbilityActivationPolicy : uint8
{
	//Try to activate the aiblity when the input is triggered
	OnInputTriggered,

	//Continually try to activate the ability while the input is active
	WhileInputActive,

	//Try to activate the passive ability when an avatar is assigned
	OnSpawn
};
UENUM(BlueprintType)
enum class EMAbilityActivationGroup : uint8
{
	//Ability runs independent of all other abilities
	Independent,

	//Ability is cancelled if another ability in the group is activated
	Exclusive_Replaceable,

	//Ability which blocks other exclusive abilities from activating
	Exclusive_Blocking
};

/**Failure reason which can be used to report failing condiitonse*/

USTRUCT(BlueprintType)
struct FMFailedReason
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Ability System")
	FGameplayTag FailedTag;

	UPROPERTY(BlueprintReadOnly, Category = "Ability System")
	FText FailureMessage;

	FMFailedReason()
	{
		FailedTag = FGameplayTag::EmptyTag;
		FailureMessage = FText::GetEmpty();
	}
};


UCLASS()
class MACHINAAUTOMADUM_API UMGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
	public:

	UMGameplayAbility(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	EMAbilityActivationPolicy GetActivationPolicy() const { return ActivationPolicy; }
	EMAbilityActivationGroup GetActivationGroup() const { return ActivationGroup; }

	// Returns true if the requested activation group is a valid transition.
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Lyra|Ability", Meta = (ExpandBoolAsExecs = "ReturnValue"))
	bool CanChangeActivationGroup(EMAbilityActivationGroup NewGroup) const;

	// Tries to change the activation group.  Returns true if it successfully changed.
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Lyra|Ability", Meta = (ExpandBoolAsExecs = "ReturnValue"))
	bool ChangeActivationGroup(EMAbilityActivationGroup NewGroup);

	//series of getters for the ability system

	//camera setters

	protected:

	//~UGameplayAbility interface
	//virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;
	//virtual void SetCanBeCanceled(bool bCanBeCanceled) override;
	//virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	//virtual void OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	//virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	//virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	//virtual bool CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	//virtual void ApplyCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const override;
	//virtual FGameplayEffectContextHandle MakeEffectContext(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo) const override;
	//virtual void ApplyAbilityTagsToGameplayEffectSpec(FGameplayEffectSpec& Spec, FGameplayAbilitySpec* AbilitySpec) const override;
	//virtual bool DoesAbilitySatisfyTagRequirements(const UAbilitySystemComponent& AbilitySystemComponent, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	//~End of UGameplayAbility interface

	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	//additional functions for related to when ability is granted or removed

	protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability System")
	EMAbilityActivationPolicy ActivationPolicy;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability System")
	EMAbilityActivationGroup ActivationGroup;
	

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ability System")
	float EnergyCost;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ability System")
    TSubclassOf<class UGameplayEffect> EnergyCostEffect;

	UFUNCTION(BlueprintCallable, Category = "Ability System")
	bool TryConsumingEnergy(float EnergyToConsume);

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);
	//TODO: Add ability costs

	//TODO: Add custom active camera mode
};
