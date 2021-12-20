// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Components/SplineComponent.h"
#include "OWPathAssetLink.generated.h"

/**
 * 
 */
UCLASS()
class OPENWORLDPLUGIN_API UOWPathAssetLink : public UObject
{
	GENERATED_BODY()
	
	UPROPERTY()
	FSplineCurves Spline;

public:
	UPROPERTY()
    TObjectPtr<class UOWPathAssetNode> LeftNode;

	UPROPERTY()
	TObjectPtr<class UOWPathAssetNode> RightNode;
};
