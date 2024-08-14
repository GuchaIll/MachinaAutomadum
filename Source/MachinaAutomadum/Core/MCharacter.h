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
#include "MCharacter.generated.h"

class UMAbilitySystemComponent;
class UMGameplayAbility;
class UMAttribute;

UCLASS()
class MACHINAAUTOMADUM_API AMCharacter : public ACharacter, public IAbilitySystemInterface, public IGameplayTagAssetInterface
{
    GENERATED_BODY()

    public:

    AMCharacter();

    UAbilitySystemComponent *GetAbilitySystemComponent() const override;

    virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;

    //UFUNCTION(BlueprintCallable, Category = "Gameplay Tag")
	//void AddTag(const FGameplayTag& Tag)
    //{
     //   OwnedTags.AddTag(Tag);
    //} 

	//UFUNCTION(BlueprintCallable, Category = "Gameplay Tag")
    //void RemoveTag(const FGameplayTag& Tag)
   // {
     //   OwnedTags.RemoveTag(Tag);
    //}


    protected:

    virtual void BeginPlay() override;

    
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
    UMAbilitySystemComponent *AbilitySystemComponent;

   
    //Any default effects that should be applied to the character
    //on spawn, for instance effects carried over from character switching
    UPROPERTY(BlueprintReadWrite, Category = "Ability")
    TArray<TSubclassOf<class UGameplayEffect>> StartupEffects;

    //Array of abilities that the character can use
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
    TArray<TSubclassOf<class UMGameplayAbility>> CharacterAbilities;

    //standard sets of abilities that all characters share (for instance death)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
    TArray<TSubclassOf<class UMGameplayAbility>> DefaultAbilities;

    float Health, MaxHealth, Energy, MaxEnergy, Armor, MaxArmor, Level, Shield, MaxShield, ShieldStack, BaseDamage;

    //Series of delegate functions for updating states
    virtual void OnHealthChanged(const FOnAttributeChangeData& Data);

	virtual void OnMaxHealthChanged(const FOnAttributeChangeData& Data);

	virtual void OnEnergyChanged(const FOnAttributeChangeData& Data);

	virtual void OnMaxEnergyChanged(const FOnAttributeChangeData& Data);

	virtual void OnArmorChanged(const FOnAttributeChangeData& Data);

	virtual void OnMaxArmorChanged(const FOnAttributeChangeData& Data);

	virtual void OnLevelChanged(const FOnAttributeChangeData& Data);

    virtual void OnShieldChanged(const FOnAttributeChangeData& Data);

    virtual void OnMaxShieldChanged(const FOnAttributeChangeData& Data);

    virtual void OnShieldStackChanged(const FOnAttributeChangeData& Data);

    virtual void OnBaseDamageChanged(const FOnAttributeChangeData& Data);

    virtual void OnDeath();

    virtual void OnDamageTaken(const FOnAttributeChangeData& Data);

    //Tags owned by the character
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	FGameplayTagContainer OwnedTags;

    FGameplayTag DeathTag;
};