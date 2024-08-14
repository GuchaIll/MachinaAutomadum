// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "../Renders/DamageTextRenderComponent.h"
#include "../AbilitySystem/Attributes/MAttributeSet.h"
#include "GameplayTagContainer.h"
#include "GameplayEffectTypes.h"
#include "../AbilitySystem/Abilities/MGameplayAbility.h"
#include "GameplayTagAssetInterface.h"
#include "MEnemy_Base.generated.h"

class UMAbilitySystemComponent;
class UMGameplayAbility;
class UMAttribute;
class UWidgetComponent;
class UMFloatingStatusBar;


//DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChanged, float, Health, float, MaxHealth);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEnergyChanged, float, Energy, float, MaxEnergy);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnArmorChanged, float, Armor, float, MaxArmor);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLevelChanged, float, Health, float, MaxHealth);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnExperienceChanged, float, Experience, float, MaxExperience);
UCLASS()
class MACHINAAUTOMADUM_API AMEnemy_Base : public ACharacter,  public IAbilitySystemInterface, public IGameplayTagAssetInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMEnemy_Base();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/**Set up initial values for attributes*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
	TSubclassOf<class UGameplayEffect> DefaultAttributeEffect;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Abilities, meta = (AllowPrivateAccess = "true"))
	UMAbilitySystemComponent *AbilitySystemComponent;
	// overriden from IAbilitySystemInterface
	UAbilitySystemComponent *GetAbilitySystemComponent() const override;

	/**Start up effects (cosmetics)*/
	UPROPERTY(BlueprintReadOnly, Category = "Abilities")
	TArray<TSubclassOf<class UGameplayEffect>> StartupEffects;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Ability", meta = (AllowPrivateAccess = "true"))
	const class UMAttributeSet* AttributeSet;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
	TArray<TSubclassOf<class UMGameplayAbility>> CharacterAbilities;

	FGameplayTag DeathTag;
	FGameplayTag EffectRemoveOnDeathTag;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "3D Text")
	UDamageTextRenderComponent* DamageTextComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI")
	TSubclassOf<class UMFloatingStatusBar> UIFloatingStatusBarClass;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "UI")
	UWidgetComponent* UIFloatingStatusBarComponent;

	UPROPERTY()
	UMFloatingStatusBar* UIFloatingStatusBar;

	virtual void OnHealthChanged(const FOnAttributeChangeData& Data);

	virtual void OnMaxHealthChanged(const FOnAttributeChangeData& Data);

	virtual void OnEnergyChanged(const FOnAttributeChangeData& Data);

	virtual void OnMaxEnergyChanged(const FOnAttributeChangeData& Data);

	virtual void OnArmorChanged(const FOnAttributeChangeData& Data);

	virtual void OnMaxArmorChanged(const FOnAttributeChangeData& Data);

	virtual void OnLevelChanged(const FOnAttributeChangeData& Data);

	UFUNCTION(BlueprintCallable, Category = "3D Text")
	void DisplayDamageText();

	UFUNCTION(BlueprintCallable, Category = "3D Text")
	void UpdateDamageText(float Damage);

	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	FGameplayTagContainer OwnedTags;

	//UFUNCTION(BlueprintCallable, Category = "Gameplay Tag")
	//void AddTag(const FGameplayTag& Tag)
    //{
     //  OwnedTags.AddTag(Tag);
    //} 

	//UFUNCTION(BlueprintCallable, Category = "Gameplay Tag")
    //void RemoveTag(const FGameplayTag& Tag)
   // {
      //  OwnedTags.RemoveTag(Tag);
    //}

};
