#include "AICharacterBase.h"

AAICharacterBase::AAICharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = AAIC_Base::StaticClass();
	
	CharacterStats = CreateDefaultSubobject<UAC_CharacterStats>("CharacterStats");
	PhysicsComponent = CreateDefaultSubobject<UAC_PhysicsComponent>("PhysicsComponent");

	HeadSocketName = "Head";

	WalkSpeed = 230.f;
	RunSpeed = 500.f;

	/*Character collisions configs*/
	GetMesh()->SetCollisionProfileName("Pawn");
	GetMesh()->SetCollisionResponseToAllChannels(ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	/*Capsule collisions config*/
	GetCapsuleComponent()->SetCollisionObjectType(ECC_WorldDynamic);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);

}

void AAICharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

void AAICharacterBase::GetActorEyesViewPoint(FVector& Location, FRotator& Rotation) const
{
	Location = GetMesh()->GetSocketLocation(HeadSocketName);
	Rotation = GetMesh()->GetSocketRotation(HeadSocketName);
}

/*Change movement speed depends of movement state*/
void AAICharacterBase::SetMovementState_Implementation(EMovementState State)
{
	switch (State)
	{
	case EMS_Idle:
		GetCharacterMovement()->MaxWalkSpeed = 0;
		break;
	case EMS_Walk:
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
		break;
	case EMS_Run:
		GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
		break;
	}
}

void AAICharacterBase::GetCombatRanges_Implementation(float& AttackRadius, float& DefendRadius)
{
	AttackRadius = this->AttackRange;
	DefendRadius = this->DefendRange;
}

bool AAICharacterBase::GetIsPreparedToAttack_Implementation()
{
	return bPreparedToAttack;
}