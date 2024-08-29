// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "MAttributeSet.generated.h"

class AActor;
class MAbilitySystemComponent;
class UObject;
class UWorld;
struct FGameplayEffectSpec;


/**
 * This macro defines a set of helper functions for accessing and initializing attributes.
 *
 * The following example of the macro:
 *		ATTRIBUTE_ACCESSORS(ULyraHealthSet, Health)
 * will create the following functions:
 *		static FGameplayAttribute GetHealthAttribute();
 *		float GetHealth() const;
 *		void SetHealth(float NewVal);
 *		void InitHealth(float NewVal);
 */
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/** 
 * Delegate used to broadcast attribute events, some of these parameters may be null on clients: 
 * @param EffectInstigator	The original instigating actor for this event
 * @param EffectCauser		The physical actor that caused the change
 * @param EffectSpec		The full effect spec for this change
 * @param EffectMagnitude	The raw magnitude, this is before clamping
 * @param OldValue			The value of the attribute before it was changed
 * @param NewValue			The value after it was changed
*/
DECLARE_MULTICAST_DELEGATE_SixParams(FLyraAttributeEvent, AActor* /*EffectInstigator*/, AActor* /*EffectCauser*/, const FGameplayEffectSpec* /*EffectSpec*/, float /*EffectMagnitude*/, float /*OldValue*/, float /*NewValue*/);

/**
 * AttributeSet is a collection of gameplay attributes, which are float values that are meant to be used for defining the basic stats of an Actor.
 * These attributes can be modified by GameplayEffects in the AbilitySystem.
 * 
 * This class is meant to be subclassed, with the subclass defining the actual attributes as UPROPERTY()s.
 * 
 * The primary way to interact with an AttributeSet is through the GetAttribute functions, which will use the UPROPERTY() reflection to find the attribute.
 * 
 * The AttributeSet also has helper functions for initializing attributes and applying GE Specs to the attributes.
 */
UCLASS()
class MACHINAAUTOMADUM_API UMAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
  	UMAttributeSet();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UPROPERTY(BlueprintReadWrite, ReplicatedUsing = OnRep_Health, Category = "Attributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UMAttributeSet, Health);

	UPROPERTY(BlueprintReadWrite, ReplicatedUsing = OnRep_MaxHealth, Category = "Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UMAttributeSet, MaxHealth);

	UPROPERTY(BlueprintReadWrite, ReplicatedUsing = OnRep_MaxArmor, Category = "Attributes")
	FGameplayAttributeData MaxArmor;
	ATTRIBUTE_ACCESSORS(UMAttributeSet, MaxArmor);

	UPROPERTY(BlueprintReadWrite, ReplicatedUsing = OnRep_Armor, Category = "Attributes")
	FGameplayAttributeData Armor;
	ATTRIBUTE_ACCESSORS(UMAttributeSet, Armor);

	UPROPERTY(BlueprintReadWrite, ReplicatedUsing = OnRep_Armor, Category = "Attributes")
	FGameplayAttributeData Shield;
	ATTRIBUTE_ACCESSORS(UMAttributeSet, Shield);

	UPROPERTY(BlueprintReadWrite, ReplicatedUsing = OnRep_Armor, Category = "Attributes")
	FGameplayAttributeData MaxShield;
	ATTRIBUTE_ACCESSORS(UMAttributeSet, MaxShield);

	UPROPERTY(BlueprintReadWrite, ReplicatedUsing = OnRep_Armor, Category = "Attributes")
	FGameplayAttributeData ShieldStack;
	ATTRIBUTE_ACCESSORS(UMAttributeSet, ShieldStack);

	UPROPERTY(BlueprintReadWrite, ReplicatedUsing = OnRep_Armor, Category = "Attributes")
	FGameplayAttributeData BaseDamage;
	ATTRIBUTE_ACCESSORS(UMAttributeSet, BaseDamage);

	//Custom execution class for calculating damage multiplier
	//At loadout and character switching
	UPROPERTY(BlueprintReadWrite, ReplicatedUsing = OnRep_Armor, Category = "Attributes")
	FGameplayAttributeData DamageMultiplier;
	ATTRIBUTE_ACCESSORS(UMAttributeSet, DamageMultiplier);

	UPROPERTY(BlueprintReadWrite, ReplicatedUsing = OnRep_Armor, Category = "Attributes")	
	FGameplayAttributeData BaseAttackSpeed;
	ATTRIBUTE_ACCESSORS(UMAttributeSet, BaseAttackSpeed);

	//Custom execution class for calculating total cooldown reduction
	//At loadout and character switching

	UPROPERTY(BlueprintReadWrite, ReplicatedUsing = OnRep_Armor, Category = "Attributes")
	FGameplayAttributeData TotalCooldownReduction;
	ATTRIBUTE_ACCESSORS(UMAttributeSet, TotalCooldownReduction);

	UPROPERTY(BlueprintReadWrite, ReplicatedUsing = OnRep_Energy, Category = "Attributes")
	FGameplayAttributeData Energy;
	ATTRIBUTE_ACCESSORS(UMAttributeSet, Energy);


	UPROPERTY(BlueprintReadWrite, ReplicatedUsing = OnRep_MaxEnergy, Category = "Attributes")
	FGameplayAttributeData MaxEnergy;
	ATTRIBUTE_ACCESSORS(UMAttributeSet, MaxEnergy);

	UPROPERTY(BlueprintReadWrite, ReplicatedUsing = OnRep_Level, Category = "Attributes")
	FGameplayAttributeData Level;
	ATTRIBUTE_ACCESSORS(UMAttributeSet, Level);

	UPROPERTY(BlueprintReadWrite, ReplicatedUsing = OnRep_Experience, Category = "Attributes")
	FGameplayAttributeData Experience;
	ATTRIBUTE_ACCESSORS(UMAttributeSet, Experience);

	UPROPERTY(BlueprintReadWrite, ReplicatedUsing = OnRep_ExperienceToNextLevel, Category = "Attributes")
	FGameplayAttributeData ExperienceToNextLevel;
	ATTRIBUTE_ACCESSORS(UMAttributeSet, ExperienceToNextLevel);

	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& OldHealth);

	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);

	UFUNCTION()
	virtual void OnRep_Armor(const FGameplayAttributeData& OldArmor);

	UFUNCTION()
	virtual void OnRep_MaxArmor(const FGameplayAttributeData& OldMaxArmor);

	UFUNCTION()
	virtual void OnRep_Energy(const FGameplayAttributeData& OldEnergy);

	UFUNCTION()
	virtual void OnRep_MaxEnergy(const FGameplayAttributeData& OldMaxEnergy);

	UFUNCTION()
	virtual void OnRep_Level(const FGameplayAttributeData& OldLevel);

	UFUNCTION()
	virtual void OnRep_Experience(const FGameplayAttributeData& OldExperience);

	UFUNCTION()
	virtual void OnRep_ExperienceToNextLevel(const FGameplayAttributeData& OldExperienceToNextLevel);

	UFUNCTION()
	virtual void OnRep_Shield(const FGameplayAttributeData& OldShield);

	UFUNCTION()
	virtual void OnRep_MaxShield(const FGameplayAttributeData& OldMaxShield);

	UFUNCTION()
	virtual void OnRep_ShieldStack(const FGameplayAttributeData& OldShieldStack);

	UFUNCTION()
	virtual void OnRep_BaseDamage(const FGameplayAttributeData& OldBaseDamage);

	UFUNCTION()
	virtual void OnRep_DamageMultiplier(const FGameplayAttributeData& OldDamageMultiplier);

	UFUNCTION()
	virtual void OnRep_BaseAttackSpeed(const FGameplayAttributeData& OldBaseAttackSpeed);

	UFUNCTION()
	virtual void OnRep_TotalCooldownReduction(const FGameplayAttributeData& OldTotalCooldownReduction);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
    void MSetHealth(float NewHealthValue);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	void MSetMaxHealth(float NewMaxHealthValue);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	void MSetArmor(float NewArmorValue);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	void MSetMaxArmor(float NewMaxArmorValue);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	void MSetEnergy(float NewEnergyValue);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	void MSetMaxEnergy(float NewMaxEnergyValue);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	void MSetLevel(int NewLevelValue);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	void MSetExperience(float NewExperienceValue);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	void MSetExperienceToNextLevel(float NewExperienceToNextLevelValue);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	void MSetShield(float NewShieldValue);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	void MSetMaxShield(float NewMaxShieldValue);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	void MSetShieldStack(float NewShieldStackValue);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	void MSetBaseDamage(float NewBaseDamageValue);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	void MSetDamageMultiplier(float NewDamageMultiplierValue);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	void MSetBaseAttackSpeed(float NewBaseAttackSpeedValue);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	void MSetTotalCooldownReduction(float NewTotalCooldownReductionValue);

	


  	UWorld* GetWorld() const override;

  //UMAbilitySystemComponent* GetOwningAbilitySystemComponent() const;

};
