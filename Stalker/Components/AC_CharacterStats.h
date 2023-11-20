#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Character.h"
#include "AC_CharacterStats.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCharacterDeathDelegate);


UENUM(BlueprintType)
enum ELimbs {
	EL_Head		UMETA(DisplayName = "Head"),
	EL_Torso	UMETA(DisplayName = "Torso"),
	EL_Arms		UMETA(DisplayName = "Arms"),
	EL_Legs		UMETA(DisplayName = "Legs")
};

USTRUCT(BlueprintType) 
struct FLimbs{
	
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TEnumAsByte<ELimbs> Limb;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta = (GetOptions = "GetMeshBones"))
	TArray<FName> Bones;

};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STALKER_API UAC_CharacterStats : public UActorComponent
{
	GENERATED_BODY()

public:	

	UAC_CharacterStats();

	virtual void BeginPlay() override;

private:

	float CurrentHealth;

	float CurrentStamina;
	
	int CurrentLevel;

	bool bIsAlive;

protected:
	
	UFUNCTION()
	TArray<FName> GetMeshBones() const {
		return Cast<ACharacter>(GetOwner())->GetMesh()->GetAllSocketNames();
	};

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 0), Category = "Health")
	float MaxHealth;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,  meta = (ClampMin = 0), Category = "Stamina")
	float MaxStamina;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 0), Category = "Stamina")
	float RefreshStaminaSpeed;
	
	UPROPERTY(BlueprintreadOnly, EditDefaultsOnly, Category = "DamageSystem")
	TArray<FLimbs> Limbs;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "DamageSystem")
	TMap<TEnumAsByte<ELimbs>, float> LimbsDamageMultipliers;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "DamageSystem")
	float DefaultMultiplier;

/*Getters*/
public:
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

public:

	UFUNCTION(BlueprintPure, BlueprintCallable)
	bool IsAlive();
	
	UFUNCTION(BlueprintCallable, Category = "DamageSystem")
	void TakeDamage(float DamageAmount, FName HitBoneName, FVector HitDirection, float ImpulseStrenght = 4000.f);

	UFUNCTION(BlueprintCallable, Category = "DamageSystem")
	float Heal(float HealValue);

	/*Decrease character stamina by value*/
	UFUNCTION(BlueprintCallable, Category = "Stamina")
	float DrainStamina(float DrainAmount);

	UFUNCTION(BlueprintCallable, Category = "Stamina")
	float RefreshStamina(float RefreshAmount);

protected:

	/*Called when character is death*/
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnCharacterDeathDelegate OnCharacterDeath;
};
