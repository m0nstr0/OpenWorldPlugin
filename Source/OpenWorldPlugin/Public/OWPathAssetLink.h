// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Components/SplineComponent.h"
#include "OWPathAssetLink.generated.h"


UENUM()
enum class EOWPathAssetDirectionType : uint8
{
	LAR = 0 UMETA(DisplayName = "L ↔ R"),
	R2L = 1 UMETA(DisplayName = "L ← R"),
	L2R = 2   UMETA(DisplayName = "L → R"),
};

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
    EOWPathAssetDirectionType Direction;

	UPROPERTY()
    TObjectPtr<class UOWPathAssetNode> LeftNode;

	UPROPERTY()
	TObjectPtr<class UOWPathAssetNode> RightNode;
};
