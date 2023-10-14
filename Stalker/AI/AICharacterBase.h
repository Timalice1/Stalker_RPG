#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AIC_Base.h"
#include "../Components/AC_CharacterStats.h"

#include "AICharacterBase.generated.h"

UCLASS()
class STALKER_API AAICharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	AAICharacterBase();

protected:
	/*Define character eyes socket*/
	virtual void GetActorEyesViewPoint(FVector& Location, FRotator& Rotation) const override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UAC_CharacterStats* CharacterStats;

	/*Character head or eyes socket name*/
	UPROPERTY(EditDefaultsOnly, Category = "Mesh")
	FName HeadSocketName;

public:
	
	/*Character specific behaviour tree*/
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UBehaviorTree* BTAsset;
};
