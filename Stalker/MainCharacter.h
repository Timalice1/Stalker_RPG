#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMOvementComponent.h"

#include "Components/AC_CharacterStats.h"
#include "Components/AC_PhysicsComponent.h"
#include "Components/CapsuleComponent.h"

#include "MainCharacter.generated.h"

UCLASS()
class STALKER_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

private:

	bool bIsSprinting;

	FTimerHandle StaminaRefresh_Handle;

public:

	AMainCharacter();

	UFUNCTION(BlueprintCallable)
	bool GetIsSprinting() const {
		return this->bIsSprinting;
	}

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UAC_CharacterStats* CharacterStats;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UAC_PhysicsComponent* PhysicsComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float WalkSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float RunSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float SprintSpeed;

protected:

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void Attack();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void StopAttack();

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void Sprint(float StaminaConsumption = .5f);

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void UnSprint();

private:

	UFUNCTION()
	void StaminaRefresh();
	
};
