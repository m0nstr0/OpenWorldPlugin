// Fill out your copyright notice in the Description page of Project Settings.


#include "OWPathAsset.h"
#include "OWPathAssetLink.h"
#include "UObject/ObjectSaveContext.h"

void UOWPathAsset::PostLoad()
{
    UObject::PostLoad();
	RefreshLinksRefs();
}

void UOWPathAsset::PreSave(FObjectPreSaveContext SaveContext)
{
	CleanDeadLinks();
    UObject::PreSave(SaveContext);
}

UOWPathAssetLink* UOWPathAsset::Link(const TWeakObjectPtr<UOWPathAssetNode>& LeftNode, const TWeakObjectPtr<UOWPathAssetNode>& RightNode, const EOWPathAssetDirectionType DirectionContext)
{
	EOWPathAssetDirectionType Direction = DirectionContext;

	if (!LeftNode.IsValid() || !RightNode.IsValid()) {
		return nullptr;
	}

	if (Nodes.Find(LeftNode.Get()) == INDEX_NONE || Nodes.Find(RightNode.Get()) == INDEX_NONE) {
		return nullptr;
	}
	
	UOWPathAssetLink* NewLink = FindLink(LeftNode.Get(), RightNode.Get());
	if (NewLink) {
		LeftNode->LinksFrom.Remove(NewLink);
		RightNode->LinksFrom.Remove(NewLink);
		if (NewLink->LeftNode == LeftNode.Get()) {
			Direction = DirectionContext;
		} else if (Direction != EOWPathAssetDirectionType::LAR) {
			Direction = DirectionContext == EOWPathAssetDirectionType::L2R ? EOWPathAssetDirectionType::R2L : EOWPathAssetDirectionType::L2R;
		}
	} else {
		NewLink = NewObject<UOWPathAssetLink>(this);
	}

	MarkPackageDirty();
	NewLink->LeftNode = LeftNode.Get();
	NewLink->RightNode = RightNode.Get();
	NewLink->Direction = Direction;

	if (Direction == EOWPathAssetDirectionType::LAR || Direction == EOWPathAssetDirectionType::L2R) {
		LeftNode->LinksFrom.AddUnique(NewLink);
	}

	if (Direction == EOWPathAssetDirectionType::LAR || Direction == EOWPathAssetDirectionType::R2L) {
		RightNode->LinksFrom.AddUnique(NewLink);
	}

    const int32 Index = Links.AddUnique(NewLink);
	return Links[Index];
}

UOWPathAssetLink* UOWPathAsset::Unlink(const TWeakObjectPtr<UOWPathAssetNode>& LeftNode, const TWeakObjectPtr<UOWPathAssetNode>& RightNode)
{
	if (!LeftNode.IsValid() || !RightNode.IsValid()) {
		return nullptr;
	}

	if (UOWPathAssetLink* LinkFound = FindLink(LeftNode.Get(), RightNode.Get())) {
		MarkPackageDirty();
		LeftNode->LinksFrom.Remove(LinkFound);
		RightNode->LinksFrom.Remove(LinkFound);
		LinkFound->LeftNode = nullptr;
		LinkFound->RightNode = nullptr;
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

void UOWPathAsset::RefreshLinksRefs()
{
	TArray<UOWPathAssetLink*> LinksToRemove;
	LinksToRemove.Reserve(Links.Num());

	for (UOWPathAssetNode* Node : Nodes) {
		Node->LinksFrom.Empty();
	}

	for (UOWPathAssetLink* Link : Links) {
		UOWPathAssetNode* LeftNode = nullptr;
		UOWPathAssetNode* RightNode = nullptr;
		
		if (!Link->LeftNode || !Link->RightNode) {
			LinksToRemove.Add(Link);
			continue;
		}

		if (const int32 LeftNodeIndex = Nodes.Find(Link->LeftNode); LeftNodeIndex != INDEX_NONE) {
			LeftNode = Nodes[LeftNodeIndex];
		}

		if (const int32 RightNodeIndex = Nodes.Find(Link->RightNode); RightNodeIndex != INDEX_NONE) {
			RightNode = Nodes[RightNodeIndex];
		}

		if (!LeftNode || !RightNode) {
			LinksToRemove.Add(Link);
			continue;
		}

		if (Link->Direction == EOWPathAssetDirectionType::LAR || Link->Direction == EOWPathAssetDirectionType::L2R) {
			LeftNode->LinksFrom.Add(Link);
		}

		if (Link->Direction == EOWPathAssetDirectionType::LAR || Link->Direction == EOWPathAssetDirectionType::R2L) {
			RightNode->LinksFrom.Add(Link);
		}
	}

	for (UOWPathAssetLink* Link : LinksToRemove) {
		Link->LeftNode = nullptr;
		Link->RightNode = nullptr;
		Links.Remove(Link);
	}

	LinksToRemove.Empty();
}

void UOWPathAsset::CleanDeadLinks()
{
	Links.RemoveAll([this](const UOWPathAssetLink* Link) {
		if (!Link->LeftNode || !Link->RightNode) {
			return true;
		}

		if (Nodes.Find(Link->LeftNode) == INDEX_NONE || Nodes.Find(Link->RightNode) == INDEX_NONE) {
			return true;
		}

		return false;
	});
}

void UOWPathAsset::RemoveNode(const TWeakObjectPtr<UOWPathAssetNode>& Node)
{
	if (!Node.IsValid()) {
	    return;
	}

	TArray<UOWPathAssetLink*> LinksToRemove;
	LinksToRemove.Reserve(Links.Num());
	for (UOWPathAssetLink* Link : Links)
	{
	    if (Link->LeftNode == Node.Get() || Link->RightNode == Node.Get()) {
			LinksToRemove.Add(Link);
	    }
	}

	MarkPackageDirty();
	for (UOWPathAssetLink* Link : LinksToRemove) {
		if (Link->LeftNode)	{
			Link->LeftNode->LinksFrom.Remove(Link);
		}
		if (Link->RightNode) {
			Link->RightNode->LinksFrom.Remove(Link);
		}
		Link->LeftNode = nullptr;
		Link->RightNode = nullptr;
		Links.Remove(Link);
	}

	LinksToRemove.Empty();
	Nodes.Remove(Node.Get());
}
