#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AIC_Base.h"
#include "../Components/AC_CharacterStats.h"
#include "AIInterface.h"

#include "AICharacterBase.generated.h"

UCLASS(Blueprintable)
class STALKER_API AAICharacterBase : public ACharacter, public IAIInterface
{
	GENERATED_BODY()

public:
	
	AAICharacterBase();

protected:
	
	/*Define character eyes socket*/
	virtual void GetActorEyesViewPoint(FVector& Location, FRotator& Rotation) const override;

	/*HAndling character taking any damage*/
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UAC_CharacterStats* CharacterStats;

	/*Character head or eyes socket name*/
	UPROPERTY(EditDefaultsOnly, Category = "Mesh")
	FName HeadSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float WalkSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float RunSpeed;

public:
	/*Character specific behaviour tree*/
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UBehaviorTree* BTAsset;
	
/*Interface function overrides*/
public:

	/*Change movement speed depends of movement state*/
	void SetMovementState_Implementation(EMovementState State) override;

};
