// Fill out your copyright notice in the Description page of Project Settings.


#include "OWPathAsset.h"
#include "OWPathAssetLink.h"

UOWPathAssetLink* UOWPathAsset::Link(const TWeakObjectPtr<UOWPathAssetNode>& LeftNode, const TWeakObjectPtr<UOWPathAssetNode>& RightNode, EOWPathAssetDirectionType DirectionContext)
{
	if (!LeftNode.IsValid() || !RightNode.IsValid()) {
		return nullptr;
	}

	UOWPathAssetLink* NewLink = FindLink(LeftNode.Get(), RightNode.Get());
	if (!NewLink) {
		NewLink = NewObject<UOWPathAssetLink>(this);
	}

	MarkPackageDirty();
	NewLink->LeftNode = LeftNode.Get();
	NewLink->RightNode = RightNode.Get();
	NewLink->Direction = DirectionContext;

	return Links.Add_GetRef(NewLink);
}

UOWPathAssetLink* UOWPathAsset::Unlink(const TWeakObjectPtr<UOWPathAssetNode>& LeftNode, const TWeakObjectPtr<UOWPathAssetNode>& RightNode)
{
	if (!LeftNode.IsValid() || !RightNode.IsValid()) {
		return nullptr;
	}

	if (UOWPathAssetLink* LinkFound = FindLink(LeftNode.Get(), RightNode.Get())) {
		MarkPackageDirty();
		Links.Remove(LinkFound);
		return LinkFound;
	}

	return nullptr;
}

UOWPathAssetLink* UOWPathAsset::FindLink(const TWeakObjectPtr<UOWPathAssetNode>& LeftNode, const TWeakObjectPtr<UOWPathAssetNode>& RightNode, EOWPathAssetDirectionType& OutDirectionContext)
{
	OutDirectionContext = EOWPathAssetDirectionType::LAR;

	if (!LeftNode.IsValid() || !RightNode.IsValid()) {
		return nullptr;
	}

	return FindLink(LeftNode.Get(), RightNode.Get(), OutDirectionContext);
}

UOWPathAssetLink* UOWPathAsset::FindLink(const UOWPathAssetNode* LeftNode, const UOWPathAssetNode* RightNode, EOWPathAssetDirectionType& OutDirectionContext)
{
	OutDirectionContext = EOWPathAssetDirectionType::LAR;
	UOWPathAssetLink* PathAssetLink = FindLink(LeftNode, RightNode);

	if (PathAssetLink != nullptr && PathAssetLink->Direction != EOWPathAssetDirectionType::LAR) {
		OutDirectionContext = PathAssetLink->Direction;
		if (PathAssetLink->LeftNode != LeftNode) {
			OutDirectionContext = PathAssetLink->Direction == EOWPathAssetDirectionType::R2L ? EOWPathAssetDirectionType::L2R : EOWPathAssetDirectionType::R2L;
		}
	}
	
    return PathAssetLink;
}

UOWPathAssetLink* UOWPathAsset::FindLink(const UOWPathAssetNode* LeftNode, const UOWPathAssetNode* RightNode)
{
	if (!LeftNode || !RightNode) {
		return nullptr;
	}

	UOWPathAssetLink** PathAssetLink = Links.FindByPredicate([LeftNode, RightNode](const UOWPathAssetLink* Link) {
		if (Link->LeftNode == LeftNode && Link->RightNode == RightNode) {
			return true;
		}
		if (Link->LeftNode == RightNode && Link->RightNode == LeftNode) {
			return true;
		}
		return false;
	});

	return PathAssetLink ? *PathAssetLink : nullptr;
}