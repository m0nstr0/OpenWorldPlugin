// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OWPathAssetEdModeActor.generated.h"

UCLASS()
class AOWPathAssetEdModeActor : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY()
	TObjectPtr<class UCapsuleComponent> CapsuleComponent;

	UPROPERTY()
	TObjectPtr<class UStaticMeshComponent> StaticMeshComponent;

public:	
	// Sets default values for this actor's properties
	AOWPathAssetEdModeActor();
};
