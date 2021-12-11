// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "OWPathAssetNode.generated.h"


/**
 * 
 */
UCLASS(BlueprintType)
class OPENWORLDPLUGIN_API UOWPathAssetNode : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector Location;

	UPROPERTY()
	TArray<TObjectPtr<class UOWPathAssetConnection>> Connections;
};
