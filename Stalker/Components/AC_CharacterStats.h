#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "GameFramework/Character.h"
#include "AC_PhysicsComponent.h"
#include "Components/CapsuleComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

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
struct FLimb{
	
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TEnumAsByte<ELimbs> Limb;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta = (GetOptions = "GetMeshBones"))
	TArray<FName> Bones;

};

USTRUCT(BlueprintType)
struct FDamageInfo{

	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "DamageSystem")
	float DamageAmount;
	
	UPROPERTY(BlueprintReadWrite, Category = "DamageSystem")
	FName HitBone;

	UPROPERTY(BlueprintReadWrite, Category = "DamageSystem")
	FVector ImpactPoint;

	UPROPERTY(BlueprintReadWrite, Category = "DamageSystem")
	FVector HitDirection;

	UPROPERTY(BlueprintReadWrite, Category = "DamageSystem")
	float ImpulseStrenght;

};

UCLASS(meta=(BlueprintSpawnableComponent))
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

public:
	
	UFUNCTION()
	FORCEINLINE TArray<FName> GetMeshBones() const {
		return Cast<ACharacter>(GetOwner())->GetMesh()->GetAllSocketNames();
	};

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 0), Category = "Health")
	float MaxHealth;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,  meta = (ClampMin = 0), Category = "Stamina")
	float MaxStamina;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 0), Category = "Stamina")
	float RefreshStaminaSpeed;
		
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "DamageSystem",
		meta = (AllowPrivateAccess = "true"))
	TArray<FLimb> Limbs;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "DamageSystem",
		meta = (ClampMin = .01f, ClampMax = 1.f))
	TMap<TEnumAsByte<ELimbs>, float> LimbsDamageMultipliers;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "DamageSystem",
		meta = (ClampMin = .0f, ClampMax = .9f))
	TMap<TEnumAsByte<ELimbs>, float> LimbsProtectionModifiers;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "DamageSystem", 
		meta = (ClampMin = .01f, ClampMax = 1.f))
	float DefaultMultiplier;

	UPROPERTY(EditDefaultsOnly, Category = "DamageSystem")
	TSubclassOf<AActor> ImpactFX_Class;

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

	UFUNCTION(BlueprintCallable, Category = "BodyParts")
	bool GetLimbByBone(FName BoneName, TEnumAsByte<ELimbs>& Limb);

public:

	UFUNCTION(BlueprintPure, BlueprintCallable)
	bool IsAlive();
	
	UFUNCTION(BlueprintCallable, Category = "DamageSystem")
	float TakeDamage(FDamageInfo DamageInfo);

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

	UFUNCTION(BlueprintCallable)
	void OnCharacterDeath_Event();

};
