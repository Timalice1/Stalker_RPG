#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PhysicsEngine/PhysicalAnimationComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/ActorComponent.h"
#include "AC_PhysicsComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STALKER_API UAC_PhysicsComponent : public UActorComponent
{
	GENERATED_BODY()

private:

	float ImpulseWeight;
	
public:

	UAC_PhysicsComponent();

public:

	UFUNCTION()
	void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void AddImpulse(FName BoneName, FVector Velocity, float ImpulseStrenght = 2000.f, float RecoverySpeed = .01f);

	UFUNCTION(BlueprintCallable)
	void SetStrenghtMultiplyer(float InStrenghtMultiplyer);

private:

	void DecreaseImpulse();

protected:

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FName RootBoneName;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FName PA_ProfileName;

	UPROPERTY()
	ACharacter* Owner;

	UPROPERTY()
	UPhysicalAnimationComponent* PhysAnimComponent;

	UPROPERTY()
	USkeletalMeshComponent* Mesh;

	UPROPERTY(BlueprintReadWrite, Category = "Variables")
	FTimerHandle ImpulseTimer;
};
