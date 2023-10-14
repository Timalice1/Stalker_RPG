#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AC_CharacterStats.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCharacterDeathDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STALKER_API UAC_CharacterStats : public UActorComponent
{
	GENERATED_BODY()

private:
	float CurrentHealth;

	float CurrentStamina;

	bool bIsAlive;

	int CurrentLevel;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 0), Category = "Health")
	float MaxHealth;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,  meta = (ClampMin = 0), Category = "Stamina")
	float MaxStamina;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 0), Category = "Stamina")
	float RefreshStaminaSpeed;

public:	
	UAC_CharacterStats();

	virtual void BeginPlay() override;

public:

	UFUNCTION(BlueprintPure, BlueprintCallable)
	bool IsAlive();
	
	/*Return character current level*/
	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Level")
	int GetCurrentLevel();

	/*Return current health value*/
	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Health")
	float GetCurrentHealth();

	/*Return health value in percent*/
	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Health")
	float GetHealthPercentage();

	/*Return current stamina value*/
	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Stamina")
	float GetCurrentStamina();
	
	/*Return stamina value in percent*/
	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Stamina")
	float GetStaminaPercentage();

	UFUNCTION(BlueprintCallable, Category = "Health")
	void DecreaseHealth(float DamageValue);

	UFUNCTION(BlueprintCallable, Category = "Health")
	float Heal(float HealValue);

	/*Decrease character stamina by value*/
	UFUNCTION(BlueprintCallable, Category = "Stamina")
	float DrainStamina(float DrainAmount);

	UFUNCTION(BlueprintCallable, Category = "Stamina")
	float RefreshStamina(float RefreshAmount);

protected:
	/*Called when character is death*/
	UPROPERTY(BlueprintAssignable)
	FOnCharacterDeathDelegate OnCharacterDeath;
};
