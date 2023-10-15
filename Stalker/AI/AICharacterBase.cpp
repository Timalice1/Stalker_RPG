#include "AICharacterBase.h"

AAICharacterBase::AAICharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	
	CharacterStats = CreateDefaultSubobject<UAC_CharacterStats>("AC_CharacterStats");

	HeadSocketName = "Head";

	WalkSpeed = 500.f;
	RunSpeed = 1000.f;
}

void AAICharacterBase::GetActorEyesViewPoint(FVector& Location, FRotator& Rotation) const
{
	Location = GetMesh()->GetSocketLocation(HeadSocketName);
	Rotation = GetMesh()->GetSocketRotation(HeadSocketName);
}

float AAICharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{

	CharacterStats->DecreaseHealth(DamageAmount);
	return DamageAmount;
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