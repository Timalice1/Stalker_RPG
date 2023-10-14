#include "AICharacterBase.h"

AAICharacterBase::AAICharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	
	CharacterStats = CreateDefaultSubobject<UAC_CharacterStats>("AC_CharacterStats");

	HeadSocketName = "Head";
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
