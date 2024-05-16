// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameplayTagContainer.h"
#include <GameplayEffectTypes.h>
#include "AbilitySystemInterface.h"
#include "../AbilitySystem/Attributes/MAttributeSet.h"
#include "../AbilitySystem/Input/PlayerGameplayAbilitiesDataAsset.h"
#include "../InventorySystem/InventorySystem.h"
#include "MPlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class UAbilitySystemComponent;

UCLASS(config = Game)
class MACHINAAUTOMADUM_API AMPlayerCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent *CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent *FollowCamera;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Abilities, meta = (AllowPrivateAccess = "true"))
	UAbilitySystemComponent *AbilitySystemComponent;

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction *DoubleJumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction *MenuAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction *InteractAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction *Ability1Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction *Ability2Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction *UltimateAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction *PrimaryAttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction *DeselectTargetAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction *LockOnTargetAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction *BlockAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction *LiftOffAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction *RangeMeleeSwitchAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction *PokeAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction *ResetCameraAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction *SwitchCharacterAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction *TauntAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction *ClimbAction;

	UPROPERTY()
	FTimerHandle DashTimerHandle;

	UPROPERTY()
	FTimerHandle SprintTimerHandle;


public:
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

	virtual void Tick(float DeltaTime) override;

	// overriden from IAbilitySystemInterface
	UAbilitySystemComponent *GetAbilitySystemComponent() const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attrbutes")
	int MaxHealth = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attrbutes")
	int MaxEnergy = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attrbutes")
	int MaxArmor = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attrbutes")
	int Level = 1;

protected:
	// Called when the game starts or when spawned

	/** Called for movement input */
	void Move(const FInputActionValue &Value);

	/** Called for looking input */
	void Look(const FInputActionValue &Value);

	/** Called for dashing input */
	void Dash();

	void StopDashing();

	/** Called for sprint input */
	void Sprint();
	void StopSprinting();

	/** Called for menu input */
	void OpenCloseMenu();

	/** Called for interacting input */
	void Interact(const FInputActionValue &Value);

	/** Called for double jump input */
	void Jump();
	void StopJumping();

	void Landed(const FHitResult &Hit);

	void StartClimb();
	void StopClimb();

	bool LineTrace(FHitResult &Hit, float Distance);
	bool WallLineTrace(FHitResult &Hit);


	//Combat Related

	void UseAbility1();
	void UseAbility2();
	void UseUltimate();

	void StartPrimaryAttack();
	void StopPrimaryAttack();

	void UsePokeAttack(); 

	void UseTaunt();

	void LockOnTarget();
	void DeselectTarget();

	void StartBlocking();
	void EndBlocking();

	void TargetLiftOff();

	void SwitchRangeMelee();

	void TryResetCamera();


protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent) override;
	
	void SwitchToCombatContext();

	void SwitchToRoamingContext();

	// To add mapping context
	virtual void BeginPlay() override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent *GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent *GetFollowCamera() const { return FollowCamera; }

	virtual void PossessedBy(AController *NewController) override;
	virtual void OnRep_PlayerState() override;
	virtual void InitializeAttributes();
	virtual void GiveDefaultAbilities();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Ability", meta = (AllowPrivateAccess = "true"))
	const class UMAttributeSet* AttributeSet;

	UPROPERTY(BlueprintReadOnly, Category = "Abilities")
	TSubclassOf<class UGameplayEffect> DefaultAttributeEffect;

	UPROPERTY(BlueprintReadOnly, Category = "Abilities")
	TArray<TSubclassOf<class UGameplayAbility>> DefaultAbilities;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AbilitySystem", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPlayerGameplayAbilitiesDataAsset> PlayerGameplayAbilitiesDataAsset;

	
	int32 InputID = 0;

	void InitAbilitySystem();

	void OnAbilitySystemInputPressed();
	void OnAbilitySystemInputReleased();

	void BindInputToAbilities(UEnhancedInputComponent* EnhancedInputComponent);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory System")
   class UInventorySystem* InventorySystem;

   UFUNCTION(BlueprintCallable, Category = "Inventory")
	void UseItem(class UItem* Item);


	FORCEINLINE UPlayerGameplayAbilitiesDataAsset* GetPlayerGameplayAbilitiesDataAsset() const { return PlayerGameplayAbilitiesDataAsset; }

};
