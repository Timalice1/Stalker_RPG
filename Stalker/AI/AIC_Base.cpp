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
	TSubclassOf<UAISense> _senseClass = UAIPerceptionSystem::GetSenseClassForStimulus(GetWorld(), Stimulus);

	if (_senseClass->GetName() == "AISense_Sight")
		HandleSightSense(TargetActor, Stimulus);
	else
		SetStateAsInvestigating(Stimulus.StimulusLocation);
}

/*Handle what to do with seen actor*/
void AAIC_Base::HandleSightSense(AActor* SeenActor, FAIStimulus Stimulus)
{
	if (Stimulus.WasSuccessfullySensed()) {
		if (Cast<ACharacter>(SeenActor))
			SetStateAsCombat((ACharacter*)SeenActor);
		else
			SetStateAsInvestigating(Stimulus.StimulusLocation);
	}
	else{
		SetStateAsInvestigating(Stimulus.StimulusLocation);
	}
}

/*Handle character passive state (roam, patrool, chill, etc.)*/
void AAIC_Base::SetStateAsPassive()
{
	if (CurrentState == ECS_Passive) return;
	BlackboardComponent->SetValueAsEnum("CharacterState", ECS_Passive);
}

/*Handle character combat state (switch to attacking)*/
void AAIC_Base::SetStateAsCombat(ACharacter* Enemy)
{	
	if (CurrentState == ECS_Combat ||
		Enemy == nullptr) return;
	
	ActiveTarget = Enemy;
	BlackboardComponent->SetValueAsObject("AttackTarget", Enemy);
	BlackboardComponent->SetValueAsEnum("CharacterState", ECS_Combat);
}

/*Handle character investigating state*/
void AAIC_Base::SetStateAsInvestigating(FVector InvestigationPoint)
{
	if (CurrentState == ECS_Investigating ||
		CurrentState == ECS_Investigating) return;
	
	BlackboardComponent->SetValueAsVector("InvestigationPoint", InvestigationPoint);
	BlackboardComponent->SetValueAsEnum("CharacterState", ECS_Investigating);
}
