// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OWPathAssetLink.h"
#include "UObject/NoExportTypes.h"
#include "OWPathAssetNode.h"
#include "OWPathAsset.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class OPENWORLDPLUGIN_API UOWPathAsset : public UObject
{
	GENERATED_BODY()

public:

	UOWPathAsset() : Height(180.f), Radius(33.f) {}

	UPROPERTY(EditAnywhere, Category = PathAsset, meta = (ClampMin = "10.0"))
	float Height;

	UPROPERTY(EditAnywhere, Category = PathAsset, meta = (ClampMin = "10.0"))
	float Radius;

	UPROPERTY()
	TArray<UOWPathAssetNode*> Nodes;

	UPROPERTY()
    TArray<class UOWPathAssetLink*> Links;

	UFUNCTION(BlueprintCallable)
	TArray<UOWPathAssetNode*> GetNodes() const
	{
		return Nodes;
	}

    UOWPathAssetLink* Link(const TWeakObjectPtr<UOWPathAssetNode>& LeftNode, const TWeakObjectPtr<UOWPathAssetNode>& RightNode, EOWPathAssetDirectionType DirectionContext);
    UOWPathAssetLink* Unlink(const TWeakObjectPtr<UOWPathAssetNode>& LeftNode, const TWeakObjectPtr<UOWPathAssetNode>& RightNode);
    class UOWPathAssetLink* FindLink(const TWeakObjectPtr<UOWPathAssetNode>& LeftNode, const TWeakObjectPtr<UOWPathAssetNode>& RightNode, EOWPathAssetDirectionType& OutDirectionContext);
    class UOWPathAssetLink* FindLink(const UOWPathAssetNode* LeftNode, const UOWPathAssetNode* RightNode, EOWPathAssetDirectionType& OutDirectionContext);
    class UOWPathAssetLink* FindLink(const UOWPathAssetNode* LeftNode, const UOWPathAssetNode* RightNode);
};