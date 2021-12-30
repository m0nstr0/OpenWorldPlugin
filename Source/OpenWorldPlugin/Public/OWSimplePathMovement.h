// This file is part of OpenWorldPlugin. Copyright Bolotaev "m0nstr0" Sergey http://bolotaev.com/

#pragma once

#include "CoreMinimal.h"
#include "OWPathAsset.h"
#include "GameFramework/MovementComponent.h"
#include "OWSimplePathMovement.generated.h"

/**
 * 
 */
UCLASS()
class OPENWORLDPLUGIN_API UOWSimplePathMovement : public UMovementComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	UOWPathAsset* PathAsset;

	UPROPERTY()
	UOWPathAssetNode* CurrentNode;

    UOWPathAssetNode* GetNextNode();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
