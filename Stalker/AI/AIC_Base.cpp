#include "AIC_Base.h"
#include "AICharacterBase.h"
#include <Kismet/KismetSystemLibrary.h>
#include "BehaviorTree/BehaviorTree.h"

AAIC_Base::AAIC_Base()
{
	AI_Perception = CreateDefaultSubobject<UAIPerceptionComponent>("PerceptionComponent");

	/*Setup sight sense config*/
	SightSense = CreateDefaultSubobject<UAISenseConfig_Sight>("SightConfig");
	SightSense->DetectionByAffiliation.bDetectEnemies = true;
	SightSense->DetectionByAffiliation.bDetectNeutrals = true;
	SightSense->DetectionByAffiliation.bDetectFriendlies = true;

	SightSense->SightRadius = 1500.f;
	SightSense->LoseSightRadius = SightSense->SightRadius + SightSense->SightRadius * .1f;
	SightSense->PeripheralVisionAngleDegrees = 70.f;
	SightSense->SetMaxAge(20.f);

	/**Setup hearing sense config*/
	HearingSense = CreateDefaultSubobject<UAISenseConfig_Hearing>("HearingConfig");
	HearingSense->DetectionByAffiliation.bDetectEnemies = true;
	HearingSense->DetectionByAffiliation.bDetectNeutrals = true;
	HearingSense->DetectionByAffiliation.bDetectFriendlies = true;

	HearingSense->HearingRange = 2000.f;
	HearingSense->bUseLoSHearing = false;
	HearingSense->SetMaxAge(20.f);

	/*Setup damage sense config*/
	DamageSense = CreateDefaultSubobject<UAISenseConfig_Damage>("DamageConfig");
	DamageSense->SetMaxAge(20.f);

	/*Register senses in perception system*/
	AI_Perception->ConfigureSense(*SightSense);
	AI_Perception->ConfigureSense(*HearingSense);
	AI_Perception->ConfigureSense(*DamageSense);
	AI_Perception->SetDominantSense(SightSense->GetSenseImplementation());
	
	/*Bind AI perception events*/
	AI_Perception->OnTargetPerceptionUpdated.AddDynamic(this, &AAIC_Base::OnTargetPerceptionUpdated);

	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>("Behavior tree");
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>("Blackboard component");
}

void AAIC_Base::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	/*Try to cast possessed pawn to base character*/
	AAICharacterBase* _character = Cast<AAICharacterBase>(InPawn);

	/*Initialize blackboard and start behavior tree*/
	if (_character != nullptr && _character->BTAsset != nullptr) {
		BlackboardComponent->InitializeBlackboard(*_character->BTAsset->BlackboardAsset);
		BehaviorTreeComponent->StartTree(*_character->BTAsset);
	}

	/*Set character state as passive by default*/
	SetStateAsPassive();
}

/*Handle sensed actor*/
void AAIC_Base::OnTargetPerceptionUpdated(AActor* TargetActor, FAIStimulus Stimulus)
{
	/**
	* Check if character see other character, then handle sight sense
	* Othervise, in case of damaging or hearing, set character state to investigating
	*/

	if (UAIPerceptionSystem::GetSenseClassForStimulus(GetWorld(), Stimulus) == UAISense_Sight::StaticClass())
		HandleSightSense(TargetActor, Stimulus);
	else
		SetStateAsInvestigating(Stimulus.StimulusLocation);
}

/*Handle what to do with seen actor*/
void AAIC_Base::HandleSightSense(AActor* SeenActor, FAIStimulus Stimulus)
{
	/*Actor seen*/
	if (Stimulus.WasSuccessfullySensed()) {
		/**
		* TODO: Calculate relationships
		* In case of seen actor is character and is enemy, switch to combat.
		* In case of friend or neytral, do nothing
		* Else, start investigating stimulus location
		*/
	
		ACharacter* _seenCharacter = Cast<ACharacter>(SeenActor);
		if (_seenCharacter != nullptr)
			SetStateAsCombat(_seenCharacter);
		else
			SetStateAsInvestigating(Stimulus.StimulusLocation);
	}

	/*Handle lost sight of actor*/
	else{
		SetStateAsPassive();
		//SetStateAsInvestigating(Stimulus.StimulusLocation);
	}

}

/*Handle character passive state (roam, patrool, chill, etc.)*/
void AAIC_Base::SetStateAsPassive()
{
	if (CurrentState == ECS_Passive) return;
	CurrentState = ECS_Passive;
}

/*Handle character combat state (switch to attacking)*/
void AAIC_Base::SetStateAsCombat(AActor* Enemy)
{	
	/*Exit if character already in combat*/
	if (CurrentState == ECS_Combat ||
		Enemy == nullptr) return;
		
	/*Change blackboard values*/
	CurrentState = ECS_Combat;
	BlackboardComponent->SetValueAsObject("AttackTarget", Enemy);
}

/*Handle character investigating state*/
void AAIC_Base::SetStateAsInvestigating(FVector InvestigationPoint)
{
	if (CurrentState == ECS_Investigating) return;
	
	CurrentState = ECS_Investigating;
	BlackboardComponent->SetValueAsVector("InvestigationPoint", InvestigationPoint);
}
