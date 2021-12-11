// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HitProxies.h"
#include "OWPathAssetNode.h"

/**
 * 
 */
struct HOWPathAssetEdModeHitProxy: public HHitProxy
{
public:
	DECLARE_HIT_PROXY();

	HOWPathAssetEdModeHitProxy(UOWPathAssetNode* InRefObject)
		: HHitProxy(HPP_UI),
		RefObject(InRefObject)
	{}

	UOWPathAssetNode* RefObject;
};
