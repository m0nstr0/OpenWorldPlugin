// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "OWPathAssetTask.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class OPENWORLDPLUGIN_API UOWPathAssetTask : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
    void OnEnter(ACharacter* Character);
};
