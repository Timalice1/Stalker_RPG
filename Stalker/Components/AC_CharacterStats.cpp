#include "AC_CharacterStats.h"
#include "AC_PhysicsComponent.h"

UAC_CharacterStats::UAC_CharacterStats()
{
	bIsAlive = true;
	MaxHealth = 100.f;
	MaxStamina = 1000.f;

	RefreshStaminaSpeed = .4f;

	/*Setup body parts damage default multipliers*/
	LimbsDamageMultipliers.Add(EL_Head, 1.f);
	LimbsDamageMultipliers.Add(EL_Torso, .8f);
	LimbsDamageMultipliers.Add(EL_Arms, .4f);
	LimbsDamageMultipliers.Add(EL_Legs, .4f);

	DefaultMultiplier = .9f;
}

void UAC_CharacterStats::BeginPlay()
{
	CurrentHealth = MaxHealth;
	CurrentStamina = MaxStamina;
}

#pragma region Getters/Setters

bool UAC_CharacterStats::IsAlive()
{
	return bIsAlive;
}

float UAC_CharacterStats::GetCurrentHealth()
{
	return CurrentHealth;
}

float UAC_CharacterStats::GetHealthPercentage()
{
	return CurrentHealth/MaxHealth;
}

float UAC_CharacterStats::GetCurrentStamina()
{
	return CurrentStamina;
}

float UAC_CharacterStats::GetStaminaPercentage()
{
	return CurrentStamina / MaxStamina;
}


int UAC_CharacterStats::GetCurrentLevel()
{
	return CurrentLevel;
}

#pragma endregion

void UAC_CharacterStats::TakeDamage(float DamageAmount, FName HitBoneName, FVector HitDirection, float ImpulseStrenght)
{
	if (!bIsAlive) 
		return;

	float _damageMultiplier = DefaultMultiplier;
	/*
	Get hited limb damage multiplier value,
	or use default multiplier if limb not found
	*/
	if(Limbs.Num() != 0)
		for (FLimbs _limb : Limbs) {
			for (FName bone : _limb.Bones) {
				if (bone == HitBoneName)
					_damageMultiplier = *LimbsDamageMultipliers.Find(_limb.Limb);
			}
		}
	/*
	Calculate final damage amount using formula:
	DamageAmount x (BoneDamageMultiplier - (BoneDamageMultiplier * BoneDefenceMultiplier)) 
	*/
	float _calculatedDamage = DamageAmount * _damageMultiplier;
	CurrentHealth -= _calculatedDamage;

	/*Apply hit impulse to selected bone*/
	UAC_PhysicsComponent* _physComponent = GetOwner()->GetComponentByClass<UAC_PhysicsComponent>();
	_physComponent->AddImpulse(HitBoneName, HitDirection, ImpulseStrenght);

	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Green, FString::Printf(TEXT("%f"), _calculatedDamage));

	/*Check if character still alive
	Call handler event on character death*/
	bIsAlive = CurrentHealth > 0;
	if (!bIsAlive) OnCharacterDeath.Broadcast();
}

float UAC_CharacterStats::Heal(float HealValue)
{
	CurrentHealth += HealValue;
	return CurrentHealth;
}

float UAC_CharacterStats::DrainStamina(float DrainAmount)
{

	CurrentStamina = FMath::Clamp(CurrentStamina -= DrainAmount, 0, MaxStamina);
	return CurrentStamina;
}

float UAC_CharacterStats::RefreshStamina(float RefreshAmount)
{
	CurrentStamina = FMath::Clamp(CurrentStamina += RefreshAmount, 0, MaxStamina);
	return CurrentStamina;
}