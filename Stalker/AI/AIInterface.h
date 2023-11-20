#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AIInterface.generated.h"

UENUM(BlueprintType, Blueprintable)
enum EMovementState {
	EMS_Idle	UMETA(DisplayName = "Idle"),
	EMS_Walk	UMETA(DisplayName = "Walk"),
	EMS_Run		UMETA(DisplayName = "Run")
};

UINTERFACE(MinimalAPI)
class UAIInterface : public UInterface
{
	GENERATED_BODY()
};

class STALKER_API IAIInterface
{
	GENERATED_BODY()

public:

	/*Change movement speed depends of movement state*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Movement")
	void SetMovementState(EMovementState State);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	void GetCombatRanges(float& AttackRadius, float& DefendRadius);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Combat")
	void PrepareToAttack();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	bool GetIsPreparedToAttack();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Combat")
	void Attack();

};