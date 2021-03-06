// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OWPathAsset.h"
#include "Tools/UEdMode.h"
#include "InteractiveTool.h"
#include "OWPathAsset_EdMode.generated.h"

/**
 * 
 */

UCLASS(Transient)
class UOWPathAsset_EdModeProperties : public UInteractiveToolPropertySet
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = Options)
	UOWPathAsset* PathAsset;
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOWEdModeObjectsChanged, const TArray<UObject*>&)
DECLARE_MULTICAST_DELEGATE_OneParam(FOWEdModePathAssetSelected, TWeakObjectPtr<UOWPathAsset>)

UCLASS(Transient)
class UOWPathAsset_EdMode : public UEdMode
{
	GENERATED_BODY()

public:
	UPROPERTY(Transient)
	TObjectPtr<UOWPathAsset_EdModeProperties> ModeProperties;

	UPROPERTY(Transient)
	TArray<TObjectPtr<UObject>> ModePropertyObjects;

	UPROPERTY(Transient)
	UOWPathAsset* PathAssetBeingEdited;

	FOWEdModeObjectsChanged OnEdModeObjectsChanged;
	FOWEdModePathAssetSelected OnPathAssetSelected;

	const static FEditorModeID EM_OpenWorldEdModeId;
	const static FName PathAssetSelect_Tool;
	const static FName PathAssetCreateNode_Tool;
    static const FName PathAssetConnection_Tool;

public:
	UOWPathAsset_EdMode();
	virtual ~UOWPathAsset_EdMode();
	virtual void Enter() override;
	virtual void Exit() override;
	void CreateToolkit() override;
	TMap<FName, TArray<TSharedPtr<FUICommandInfo>>> GetModeCommands() const override;
	bool UsesToolkits() const override { return true; }
	virtual bool IsSelectionAllowed(AActor* InActor, bool bInSelection) const { return false; }

protected:
	virtual void BindCommands() override {};
	void OnToolStarted(UInteractiveToolManager* Manager, UInteractiveTool* Tool) override;
	void OnToolEnded(UInteractiveToolManager* Manager, UInteractiveTool* Tool) override;

	void AddModePropertyObject(UObject* InObject);
	void ClearModePropertyObject();
	void ReplaceModePropertyObject(UObject* OldObject, UObject* WithObject);

	void ModeTick(float DeltaTime) override;
    void DeactivateAllTools() const;
    bool ShouldToolStartBeAllowed(const FString& ToolIdentifier) const override;
};