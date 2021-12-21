// Fill out your copyright notice in the Description page of Project Settings.

#include "OWPathAssetHitProxies.h"
#include "OWPathAssetNode.h"
#include "OWPathAssetLink.h"

IMPLEMENT_HIT_PROXY(HOWPathAssetNodeHitProxy, HHitProxy);
IMPLEMENT_HIT_PROXY(HOWPathAssetLinkHitProxy, HHitProxy);

HOWPathAssetNodeHitProxy::HOWPathAssetNodeHitProxy(UOWPathAssetNode* InRefObject) : 
	HHitProxy(HPP_UI),
	RefObject(InRefObject) {}

HOWPathAssetLinkHitProxy::HOWPathAssetLinkHitProxy(UOWPathAssetLink* InRefObject) :
	HHitProxy(HPP_UI),
	RefObject(InRefObject) {}

