// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "../MGameplayAbility.h"
#include "GA_Stumbling.generated.h"



UCLASS()
class MACHINAAUTOMADUM_API UGA_Stumbling : public UMGameplayAbility
{
    GENERATED_BODY()
    
    public:

    UGA_Stumbling();

    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
    float StumbleDistance;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
    float StumbleSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
    float StumbleCooldown;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
    UAnimMontage* StumbleUpwardMontage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
    UAnimMontage* StumbleDownwardMontage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
    UAnimMontage* StumbleForwardMontage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
    UAnimMontage* StumbleBackwardMontage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
    UAnimMontage* StumbleLeftMontage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
    UAnimMontage* StumbleRightMontage;

    UAnimMontage* GetStumbleMontage(FVector HitDirection);


};