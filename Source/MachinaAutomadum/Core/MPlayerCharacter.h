// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameplayTagContainer.h"
#include "GameplayEffectTypes.h"
#include "AbilitySystemInterface.h"
#include "../AbilitySystem/Attributes/MAttributeSet.h"
#include "../AbilitySystem/Abilities/MGameplayAbility.h"
#include "../AbilitySystem/Input/PlayerGameplayAbilitiesDataAsset.h"
#include "../InventorySystem/InventorySystem.h"
#include "../Renders/Widgets/PlayerHUDWidget.h"
#include "Components/WidgetComponent.h"
#include "Blueprint/UserWidget.h"
#include "GameplayTagAssetInterface.h"
#include "GameplayEffectTypes.h"
#include "Kismet/KismetSystemLibrary.h"

#include "MPlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class UMAbilitySystemComponent;
class UMTeamManager;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCharacterDeathDelegate, AMPlayerCharacter*, Character);
UCLASS(config = Game)
class MACHINAAUTOMADUM_API AMPlayerCharacter : public ACharacter, public IAbilitySystemInterface, public IGameplayTagAssetInterface
{
	GENERATED_BODY()
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent *CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent *FollowCamera;

	
	/** MappingContext*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> RoamingMCMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> CombatMCMappingContext;


	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction *JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction *MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction *LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction *DashAction;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	//UInputAction *DoubleJumpAction;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	//UInputAction *MenuAction;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	//UInputAction *InteractAction;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	// UInputAction *Ability1Action;

	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	// UInputAction *Ability2Action;

	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	// UInputAction *UltimateAction;

	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	// UInputAction *PrimaryAttackAction;

	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	// UInputAction *DeselectTargetAction;

	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	// UInputAction *LockOnTargetAction;

	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	// UInputAction *BlockAction;

	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	// UInputAction *LiftOffAction;

	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	// UInputAction *RangeMeleeSwitchAction;

	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	// UInputAction *PokeAction;

	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	// UInputAction *ResetCameraAction;

	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	// UInputAction *SwitchCharacterAction;

	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	// UInputAction *TauntAction;

	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	// UInputAction *ClimbAction;

	// UPROPERTY()
	// FTimerHandle DashTimerHandle;

	// UPROPERTY()
	// FTimerHandle SprintTimerHandle;


public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Abilities, meta = (AllowPrivateAccess = "true"))
	UMAbilitySystemComponent *AbilitySystemComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float DashDistance = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float DashCooldown = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float SprintSpeed = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	int32 JumpCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	int32 MaxJumpCount = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float ClimbSpeed = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivate))
	float WallLineTraceDistance = 100.0f;

private:
	bool bIsDashing = false;

	bool bIsSprinting = false;

	bool bIsClimbing = false;

public:
	// Sets default values for this character's properties
	AMPlayerCharacter();

	virtual void SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent) override;
	

	virtual void Tick(float DeltaTime) override;

	// overriden from IAbilitySystemInterface
	UAbilitySystemComponent *GetAbilitySystemComponent() const override;

	UPROPERTY(BlueprintAssignable, Category = "Death")
	FCharacterDeathDelegate OnCharacterDeath;
	
	UPROPERTY(BlueprintReadOnly, Category = "Player Info")
	FText CharacterName;





protected:
	// Called when the game starts or when spawned

	// /** Called for movement input */
	 void Move(const FInputActionValue &Value);

	// /** Called for looking input */
	 void Look(const FInputActionValue &Value);

	// /** Called for dashing input */
	 void Dash();

	 void StopDashing();

	// /** Called for sprint input */
	 void Sprint();
	 void StopSprinting();

	// /** Called for menu input */
	 void OpenCloseMenu();

	// /** Called for interacting input */
	 void Interact(const FInputActionValue &Value);

	// /** Called for double jump input */
	 void Jump();
	 void StopJumping();

	 void Landed(const FHitResult &Hit);

	 //void StartClimb();
	//void StopClimb();

	// bool LineTrace(FHitResult &Hit, float Distance);
	// bool WallLineTrace(FHitResult &Hit);



protected:
	// APawn interface
	
	void SwitchToCombatContext();

	void SwitchToRoamingContext();

	// To add mapping context
	virtual void BeginPlay() override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent *GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent *GetFollowCamera() const { return FollowCamera; }

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Team", meta = (AllowPrivateAccess = "true"))
	UMTeamManager *TeamManager;

	/** Ability System Set up */
	virtual void PossessedBy(AController *NewController) override;
	virtual void OnRep_PlayerState() override;
	//virtual void InitializeAttributes();
	virtual void GiveDefaultAbilities();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Ability", meta = (AllowPrivateAccess = "true"))
	const class UMAttributeSet* AttributeSet;

	/**Set up initial values for attributes*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
	TSubclassOf<class UGameplayEffect> DefaultAttributeEffect;

	/**Start up effects (cosmetics)*/
	UPROPERTY(BlueprintReadOnly, Category = "Abilities")
	TArray<TSubclassOf<class UGameplayEffect>> StartupEffects;

	/**Default set of abilities given to all characters*/
	UPROPERTY(BlueprintReadOnly, Category = "Abilities")
	TArray<TSubclassOf<class UMGameplayAbility>> DefaultAbilities;

	UPROPERTY(BlueprintReadOnly, Category = "Abilities")
	TArray<TSubclassOf<class UMGameplayAbility>> CharacterAbilities;

	/**Character Dependent Ability Bindings */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AbilitySystem", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPlayerGameplayAbilitiesDataAsset> PlayerGameplayAbilitiesDataAsset;

	int32 InputID = 1;

	void InitAbilitySystem();

	void OnAbilitySystemInputPressed(const FInputActionValue& Value, int32 CurrentInputID);
	void OnAbilitySystemInputReleased(const FInputActionValue& Value, int32 CurrentInputID);

	void BindInputToAbilities(UEnhancedInputComponent* EnhancedInputComponent);

    int32 GetAbilityLevel(int32 AbilityInputID) const;

	virtual void RemoveCharacterAbilities();

	virtual void AddStartupEffects();

	/* Death Related Logic */

	void Die();

	UFUNCTION(BlueprintCallable, Category = "Death")
	virtual void FinishDying();

	FGameplayTag DeathTag;
	FGameplayTag EffectRemoveOnDeathTag;

	UPROPERTY(BlueprintReadOnly, Category = "Death")
	UAnimMontage* DeathMontage;

	UFUNCTION(BlueprintCallable, Category = "Death")
	bool IsAlive() const;

	/*Attribute Delegate*/
	UFUNCTION(BlueprintImplementableEvent, Category = "Attributes")
	void HealthChanged(float NewHealth, float oldHealth, float MaxHealth);
	 
	UFUNCTION(BlueprintImplementableEvent, Category = "Attributes")
	void MaxHealthChanged(float NewMaxHealth, float oldMaxHealth, float Health);

	UFUNCTION(BlueprintImplementableEvent, Category = "Attributes")
	void EnergyChanged(float NewEnergy, float oldEnergy, float MaxEnergy);

	UFUNCTION(BlueprintImplementableEvent, Category = "Attributes")
	void MaxEnergyChanged(float NewMaxEnergy, float oldMaxEnergy, float Energy);

	UFUNCTION(BlueprintImplementableEvent, Category = "Attributes")
	void ArmorChanged(float NewArmor, float oldArmor, float MaxArmor);

	UFUNCTION(BlueprintImplementableEvent, Category = "Attributes")	
	void MaxArmorChanged(float NewMaxArmor, float oldMaxArmor, float Armor);

	UFUNCTION(BlueprintImplementableEvent, Category = "Attributes")
	void LevelChanged(int32 NewLevel, int32 oldLevel);

	UFUNCTION(BlueprintImplementableEvent, Category = "Attributes")
	void ExperienceChanged(float NewExperience, float oldExperience, float MaxExperience);

	UFUNCTION(BlueprintImplementableEvent, Category = "Attributes")
	void OnCharacterDeath_BP();
	
	virtual void OnHealthChanged(const FOnAttributeChangeData& Data);

	virtual void OnMaxHealthChanged(const FOnAttributeChangeData& Data);

	virtual void OnEnergyChanged(const FOnAttributeChangeData& Data);

	virtual void OnMaxEnergyChanged(const FOnAttributeChangeData& Data);

	virtual void OnArmorChanged(const FOnAttributeChangeData& Data);

	virtual void OnMaxArmorChanged(const FOnAttributeChangeData& Data);

	virtual void OnLevelChanged(const FOnAttributeChangeData& Data);

	virtual void OnExperienceChanged(const FOnAttributeChangeData& Data);

	int GetCharacterLevel();

	/**HUD */	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<class UPlayerHUDWidget> PlayerHUDWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	UPlayerHUDWidget *PlayerHUD;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "UI")
	UWidgetComponent* PlayerHUDComponent;

	/** Inventory System */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory System")
   class UInventorySystem* InventorySystem;

   UFUNCTION(BlueprintCallable, Category = "Inventory")
	void UseItem(class UItem* Item);

	FORCEINLINE UPlayerGameplayAbilitiesDataAsset* GetPlayerGameplayAbilitiesDataAsset() const { return PlayerGameplayAbilitiesDataAsset; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilitySystem", meta = (AllowPrivateAccess = "true"))
	float PassiveMovementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilitySystem", meta = (AllowPrivateAccess = "true"))	
	float CombatMovementSpeed;

	void CycleTarget(bool Clockwise = true);

	UFUNCTION(BlueprintCallable, Category = "AbilitySystem")
	void CycleTargetClockwise();

	UFUNCTION(BlueprintCallable, Category = "AbilitySystem")
	void CycleTargetCounterClockwise();

	TArray<class AActor*> NearbyEnemies;
	FVector InputDirection;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
		bool TargetLocked;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
		AActor* Target;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
	float TargetLockDistance;

	UFUNCTION()
		void OnEnemyDetectionBeginOverlap(UPrimitiveComponent* OverlappedComp,
			AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnEnemyDetectionEndOverlap(class UPrimitiveComponent*
			OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex);


	void FocusTarget();
	void ToggleCombatMode();
	void SetInCombat(bool bInCombat);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
		class UStaticMeshComponent* Weapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
		class USphereComponent* EnemyDetectionCollider;

	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	FGameplayTagContainer OwnedTags;

	//UFUNCTION(BlueprintCallable, Category = "Gameplay Tag")
	//void AddTag(const FGameplayTag& Tag)
    //{
       // OwnedTags.AddTag(Tag);
    //} 

	//UFUNCTION(BlueprintCallable, Category = "Gameplay Tag")
   // void RemoveTag(const FGameplayTag& Tag)
    //{
      //  OwnedTags.RemoveTag(Tag);
    //}
};
