// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OWPathAssetLink.h"
#include "UObject/NoExportTypes.h"
#include "OWPathAssetNode.generated.h"


class UOWPathAssetTask;
/**
 * 
 */
UCLASS(BlueprintType)
class OPENWORLDPLUGIN_API UOWPathAssetNode : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, meta = (DisplayName = "Behaviour"))
    TSubclassOf<class UOWPathAssetTask> BehaviourHandlerClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector Location;

	TArray<TWeakObjectPtr<class UOWPathAssetLink>> LinksFrom;

	virtual void PostLoad() override;

	UFUNCTION(BlueprintCallable)
    UOWPathAssetTask* GetBehaviourHandler();

    UFUNCTION(BlueprintCallable)
	int32 GetLinksNum() const { return LinksFrom.Num(); }

	UFUNCTION(BlueprintCallable)
	class UOWPathAssetNode* GetLinkAt(int32 Index) { return LinksFrom[Index]->LeftNode == this ? LinksFrom[Index]->RightNode : LinksFrom[Index]->LeftNode; }

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif //WITH_EDITOR

private:
	UPROPERTY(Transient)
	class UOWPathAssetTask* BehaviourHandler;

	void InitBehaviourHandler();
};
