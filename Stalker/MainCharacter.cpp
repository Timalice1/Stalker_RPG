#include "MainCharacter.h"

AMainCharacter::AMainCharacter()
{
	CharacterStats = CreateDefaultSubobject<UAC_CharacterStats>("CharacterStats");
	PhysicsComponent = CreateDefaultSubobject<UAC_PhysicsComponent>("PHysicsComponent");

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

void AMainCharacter::Attack()
{

}

void AMainCharacter::StopAttack()
{

}

void AMainCharacter::Sprint(float StaminaConsumption)
{
	if ((CharacterStats->GetCurrentStamina() <= CharacterStats->GetStaminaThresshold()) ||
		GetVelocity().Length() == 0) {
		return;
	}
	
	bIsSprinting = true;
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	
	float staminaLeft = CharacterStats->DrainStamina(StaminaConsumption);
	if (staminaLeft <= CharacterStats->GetStaminaThresshold())
		UnSprint();
}

void AMainCharacter::UnSprint() {
	bIsSprinting = false;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	//Start refreshing stamina
	GetWorldTimerManager().ClearTimer(StaminaRefresh_Handle);
	FTimerHandle staminaRefreshDelay_Handler;
	GetWorldTimerManager().SetTimer(staminaRefreshDelay_Handler, this, &AMainCharacter::StaminaRefresh, CharacterStats->GetStaminaRefreshDelay(), false);
}

void AMainCharacter::StaminaRefresh()
{
	GetWorldTimerManager().SetTimer(StaminaRefresh_Handle, [&] {
		CharacterStats->RefreshStamina(1.f);
	}, CharacterStats->GetStaminaRefreshSpeed(), true);
}