#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Components/AC_CharacterStats.h"
#include "Components/CapsuleComponent.h"

#include "MainCharacter.generated.h"

UCLASS()
class STALKER_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	AMainCharacter();

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UAC_CharacterStats* CharacterStats;
	
};
