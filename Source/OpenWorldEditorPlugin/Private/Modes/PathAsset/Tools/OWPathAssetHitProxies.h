// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HitProxies.h"


/**
 * 
 */
struct HOWPathAssetNodeHitProxy final : public HHitProxy
{
public:
	DECLARE_HIT_PROXY();

	HOWPathAssetNodeHitProxy(class UOWPathAssetNode* InRefObject);

	class UOWPathAssetNode* RefObject;
};

/**
 *
 */
struct HOWPathAssetLinkHitProxy final : public HHitProxy
{
public:
	DECLARE_HIT_PROXY();

	HOWPathAssetLinkHitProxy(class UOWPathAssetLink* InRefObject);

	class UOWPathAssetLink* RefObject;
};
