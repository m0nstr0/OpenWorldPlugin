// Fill out your copyright notice in the Description page of Project Settings.


#include "OWPathAssetNode.h"
#include "OWPathAssetLink.h"
#include "OWPathAssetTask.h"

void UOWPathAssetNode::PostLoad()
{
    UObject::PostLoad();

    InitBehaviourHandler();
}


void UOWPathAssetNode::InitBehaviourHandler()
{
	if (BehaviourHandlerClass) {
		BehaviourHandler = BehaviourHandlerClass != nullptr ? NewObject<UOWPathAssetTask>(GetTransientPackage(), BehaviourHandlerClass) : NewObject<UOWPathAssetTask>(GetTransientPackage());
	}
}

UOWPathAssetTask* UOWPathAssetNode::GetBehaviourHandler()
{
    return BehaviourHandler;
}

#if WITH_EDITOR
void UOWPathAssetNode::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    UObject::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.Property != nullptr) {
		const FName PropertyName(PropertyChangedEvent.Property->GetFName());
		if (PropertyName == GET_MEMBER_NAME_CHECKED(UOWPathAssetNode, BehaviourHandlerClass)) {
			InitBehaviourHandler();
		}
	}
}
#endif

