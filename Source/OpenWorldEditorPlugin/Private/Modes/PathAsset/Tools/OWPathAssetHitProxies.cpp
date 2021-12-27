// Fill out your copyright notice in the Description page of Project Settings.

#include "OWPathAssetHitProxies.h"
#include "OWPathAssetNode.h"
#include "OWPathAssetLink.h"

IMPLEMENT_HIT_PROXY(HOWPathAsset_NodeHitProxy, HHitProxy);
IMPLEMENT_HIT_PROXY(HOWPathAsset_LinkHitProxy, HHitProxy);

HOWPathAsset_NodeHitProxy::HOWPathAsset_NodeHitProxy(UOWPathAssetNode* InRefObject) : 
	HHitProxy(HPP_UI),
	RefObject(InRefObject) {}

HOWPathAsset_LinkHitProxy::HOWPathAsset_LinkHitProxy(UOWPathAssetLink* InRefObject) :
	HHitProxy(HPP_UI),
	RefObject(InRefObject) {}

