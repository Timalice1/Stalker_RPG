#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "AIC_Base.h"
#include "../Components/AC_CharacterStats.h"
#include "../Components/AC_PhysicsComponent.h"
#include "AIInterface.h"

#include "AICharacterBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAttackFinishedDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPreparationFinishedDelegate);

UCLASS(Blueprintable)
class STALKER_API AAICharacterBase : public ACharacter, public IAIInterface
{
	GENERATED_BODY()

public:

	AAICharacterBase();

	void BeginPlay() override;

protected:
	
	/*Define character eyes socket*/
	void GetActorEyesViewPoint(FVector& Location, FRotator& Rotation) const override;

	
protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UAC_CharacterStats* CharacterStats;

	UPROPERTY(EditDefaultsOnly, BlueprintreadOnly, Category = "Components")
	UAC_PhysicsComponent* PhysicsComponent;

	/*Character head or eyes socket name*/
	UPROPERTY(EditDefaultsOnly, Category = "Mesh")
	FName HeadSocketName;

	/*Called when character finish attack event (ex. completed or interrupted)*/
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnAttackFinishedDelegate OnAttackFinished;

	/*Called when character finished attack preparation*/
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnPreparationFinishedDelegate OnPreparationFinished;
	
/*Character properties*/
protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float WalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float RunSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float AttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float DefendRange;

public:

	/*Character specific behaviour tree*/
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UBehaviorTree* BTAsset;

	UPROPERTY(BlueprintReadWrite, Category = "Combat")
	bool bPreparedToAttack = false;
	
/*Interface function overrides*/
public:

	/*Change movement speed depends of movement state*/
	void SetMovementState_Implementation(EMovementState State) override;

	void GetCombatRanges_Implementation(float& AttackRadius, float& DefendRadius) override;

	bool GetIsPreparedToAttack_Implementation() override;
};
