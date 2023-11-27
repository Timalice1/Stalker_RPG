#include "AC_CharacterStats.h"
#include "AC_PhysicsComponent.h"
#include <Kismet/KismetMathLibrary.h>
#include "UObject/ConstructorHelpers.h"

UAC_CharacterStats::UAC_CharacterStats()
{
	bIsAlive = true;
	MaxHealth = 100.f;
	MaxStamina = 1000.f;

	RefreshStaminaSpeed = .4f;

	/*Setup default body parts damage multipliers*/
	LimbsDamageMultipliers.Add(EL_Head, .24f);
	LimbsDamageMultipliers.Add(EL_Torso, .8f);
	LimbsDamageMultipliers.Add(EL_Arms, .3f);
	LimbsDamageMultipliers.Add(EL_Legs, .3f);

	DefaultMultiplier = .01f;

	/*Define default body parts protection values*/
	LimbsProtectionModifiers.Add(EL_Head, .0f);
	LimbsProtectionModifiers.Add(EL_Torso, .0f);
	LimbsProtectionModifiers.Add(EL_Arms, .0f);
	LimbsProtectionModifiers.Add(EL_Legs, .0f);
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

bool UAC_CharacterStats::GetLimbByBone(FName BoneName, TEnumAsByte<ELimbs>& Limb)
{
	if (Limbs.IsEmpty()) 
		return false;

	for (FLimb limb : Limbs) {
		for (FName bone : limb.Bones) {
			if (bone == BoneName) {
				Limb = limb.Limb;
				return true;
			}
		}
	}

	return false;
}

#pragma endregion

float UAC_CharacterStats::TakeDamage(FDamageInfo DamageInfo)
{
	/*
	Get hited limb damage multiplier and protection value,
	or use default values if limb not found
	*/
	float _damageMultiplier = DefaultMultiplier;
	float _protectionValue = .0f;
	TEnumAsByte<ELimbs> _limb;
	if (GetLimbByBone(DamageInfo.HitBone, _limb)) {
		_damageMultiplier = *LimbsDamageMultipliers.Find(_limb);
		_protectionValue = *LimbsProtectionModifiers.Find(_limb);
	}

	/*
	Calculate final damage amount using formula:
	DamageAmount x (BoneDamageMultiplier - (BoneDamageMultiplier * BoneProtectionValue)) 
	*/
	float _calculatedDamage = DamageInfo.DamageAmount * _damageMultiplier;
	
	/*Apply protection modifiers to taken damage*/
	_calculatedDamage -= _calculatedDamage * _protectionValue;
	if(bIsAlive)
		CurrentHealth -= _calculatedDamage;

	/**
	Spawn blood fx
	TODO:
	Depends of applied damage amount, and also current equipment (because on exo blood sprites might be look weird)
	The more damage, the more blood
	!!!Use phys materials for that!!!
	*/

	FVector _loc = DamageInfo.ImpactPoint;
	FRotator _rot = UKismetMathLibrary::MakeRotFromX(DamageInfo.HitDirection * -1.f);
	GetWorld()->SpawnActor<AActor>(BloodFX_Class, _loc, _rot);
	
	/*Apply impulse to the hitted bone*/
	auto* _physComp = GetOwner()->FindComponentByClass<UAC_PhysicsComponent>();
	_physComp->AddImpulse(DamageInfo.HitBone, DamageInfo.HitDirection, DamageInfo.ImpulseStrenght);

	/** 
	TODO: 
	If armor equipped, decrease durability value by some amount. 
	Also decreases armor protection modifiers 
	*/

	/*Check if character still alive
	Call handler event on character death*/
	bIsAlive = CurrentHealth > 0;
	if (!bIsAlive) OnCharacterDeath.Broadcast();

	return _calculatedDamage;
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