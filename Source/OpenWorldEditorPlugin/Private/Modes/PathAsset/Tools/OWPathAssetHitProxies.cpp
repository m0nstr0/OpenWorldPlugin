// Fill out your copyright notice in the Description page of Project Settings.

#include "OWPathAssetHitProxies.h"
#include "OWPathAssetNode.h"

IMPLEMENT_HIT_PROXY(HOWPathAssetNodeHitProxy, HHitProxy);

HOWPathAssetNodeHitProxy::HOWPathAssetNodeHitProxy(class UOWPathAssetNode* InRefObject) : 
	HHitProxy(HPP_UI),
	RefObject(InRefObject) {}

