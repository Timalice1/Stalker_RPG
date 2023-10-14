#include "AC_CharacterStats.h"

UAC_CharacterStats::UAC_CharacterStats()
{
	MaxHealth = 2000.f;
	MaxStamina = 1000.f;

	RefreshStaminaSpeed = .4f;

	bIsAlive = true;
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

void UAC_CharacterStats::DecreaseHealth(float DamageValue)
{
	if (!bIsAlive) return;

	CurrentHealth -= DamageValue;

	//Check if character still alive
	bIsAlive = CurrentHealth > 0;

	//Call handler event on character death
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