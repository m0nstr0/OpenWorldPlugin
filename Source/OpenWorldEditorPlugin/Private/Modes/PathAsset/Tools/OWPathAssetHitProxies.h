// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HitProxies.h"


/**
 * 
 */
struct HOWPathAsset_NodeHitProxy final : public HHitProxy
{
public:
	DECLARE_HIT_PROXY();

	HOWPathAsset_NodeHitProxy(class UOWPathAssetNode* InRefObject);

	class UOWPathAssetNode* RefObject;
};

/**
 *
 */
struct HOWPathAsset_LinkHitProxy final : public HHitProxy
{
public:
	DECLARE_HIT_PROXY();

	HOWPathAsset_LinkHitProxy(class UOWPathAssetLink* InRefObject);

	class UOWPathAssetLink* RefObject;
};
