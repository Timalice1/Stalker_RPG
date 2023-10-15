#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISEnseConfig_Sight.h"
#include "Perception/AISEnseConfig_Hearing.h"
#include "Perception/AISEnseConfig_Damage.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "AIC_Base.generated.h"

/*Characters active states enumeration*/
UENUM(BlueprintType)
enum ECharacterStates {
	ECS_Passive			UMETA(DisplayName = "Passive"),
	ECS_Investigating	UMETA(DisplayName = "Investigating"),
	ECS_Combat			UMETA(DisplayName = "Combat")
};

UCLASS()
class STALKER_API AAIC_Base : public AAIController
{
	GENERATED_BODY()

public:
	AAIC_Base();

	void OnPossess(APawn* Pawn) override;

	/*Active character state*/
	UPROPERTY(BlueprintReadOnly, Category = "States")
	TEnumAsByte<ECharacterStates> CurrentState;

	/*Active attack target*/
	UPROPERTY(BlueprintReadOnly, Category = "States | Combat")
	ACharacter* ActiveTarget;

/*Components*/
protected:

	UPROPERTY(EditDefaultsOnly, Category = Perception)
	UAIPerceptionComponent* AI_Perception;

	UPROPERTY()
	UAISenseConfig_Sight* SightSense;

	UPROPERTY()
	UAISenseConfig_Hearing* HearingSense;
	
	UPROPERTY()
	UAISenseConfig_Damage* DamageSense;

	UPROPERTY()
	class UBehaviorTreeComponent* BehaviorTreeComponent;

	UPROPERTY()
	class UBlackboardComponent* BlackboardComponent;

/*Methods*/
protected:

	/*Handle sensed actor*/
	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* TargetActor, FAIStimulus Stimulus);

	/*Handle character passive state (roam, patrool, chill, etc.)*/
	UFUNCTION(BlueprintCallable, Category = "States")
 	void SetStateAsPassive();

	/*Handle character combat state (switch to attacking)*/
	UFUNCTION(BlueprintCallable, Category = "States")
	void SetStateAsCombat(AActor* Enemy);
	
	/*Handle character investigating state*/
	UFUNCTION(BlueprintCallable, Category = "States")
	void SetStateAsInvestigating(FVector InvestigationPoint);

	/*Handle what to do with seen actor*/
	void HandleSightSense(AActor* SeenActor, FAIStimulus Stimulus);
};