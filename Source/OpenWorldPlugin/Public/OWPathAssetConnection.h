// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Components/SplineComponent.h"
#include "OWPathAssetConnection.generated.h"

/**
 * 
 */
UCLASS()
class OPENWORLDPLUGIN_API UOWPathAssetConnection : public UObject
{
	GENERATED_BODY()
	
	UPROPERTY()
	FSplineCurves Spline;

	UPROPERTY()
	TArray<TObjectPtr<class UOWPathAssetNode>> Nodes;
};
