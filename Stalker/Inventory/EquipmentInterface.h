#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EquipmentInterface.generated.h"

UINTERFACE(MinimalAPI)
class UEquipmentInterface : public UInterface
{
	GENERATED_BODY()
};

class STALKER_API IEquipmentInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	float GetDurability();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	float SetDurability(float ChangeAmount);
		

};
