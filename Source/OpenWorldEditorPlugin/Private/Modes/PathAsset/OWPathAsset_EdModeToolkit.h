// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OWPathAssetLink.h"
#include "Toolkits/BaseToolkit.h"
#include "Tools/OWPathAsset_LinkTool.h"
#include "Widgets/SWidget.h"

/**
 *
 */
class FOWPathAsset_EdModeToolkit final : public FModeToolkit
{
public:
    virtual void Init(const TSharedPtr<IToolkitHost>& InitToolkitHost, TWeakObjectPtr<UEdMode> InOwningMode) override;
	virtual FName GetToolkitFName() const override { return FName("OpenWorldEdMode"); }
	virtual FText GetBaseToolkitName() const override { return FText::FromString("OpenWorldEdMode Toolkit"); }
	virtual void GetToolPaletteNames(TArray<FName>& PaletteNames) const override;
    virtual void OnToolStarted(UInteractiveToolManager* Manager, UInteractiveTool* Tool) override;
    virtual void OnToolEnded(UInteractiveToolManager* Manager, UInteractiveTool* Tool) override;
	
	void OnConnectionToolNodesSelected(const TWeakObjectPtr<UOWPathAssetNode>& LeftNode, const TWeakObjectPtr<UOWPathAssetNode>& RightNode, bool HaveLink, EOWPathAssetDirectionType
                                       Direction);
    void ConnectionToolProcessSelectedNodes(const bool IsNeedToLink) const;
private:
	void CreateViewportOverlayWidgetForConnectionTool();
	void UpdateModeProperties(const TArray<UObject*>& Objects) const;
	void UpdateActiveToolProperties(UInteractiveTool* Tool) const;

private:
	TSharedPtr<SWidget> ViewportOverlayWidgetForConnectionTool;
    bool IsTwoNodesSelectedToLink = false;
	bool IsTwoNodesHaveLink = false;

    struct FLinkDirectionType { FString Text; EOWPathAssetDirectionType Direction; };
	TArray<TSharedPtr<FLinkDirectionType>> LinkDirectionTypeItems;
	TSharedPtr<STextBlock> LinkDirectionTypeTextWidget;
	TSharedPtr< SComboBox< TSharedPtr<FLinkDirectionType> > > LinkDirectionTypeComboBox;
};