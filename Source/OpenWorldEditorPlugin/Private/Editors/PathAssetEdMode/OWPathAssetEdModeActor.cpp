// Fill out your copyright notice in the Description page of Project Settings.


#include "Editors/PathAssetEdMode/OWPathAssetEdModeActor.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include <Components/SplineComponent.h>

// Sets default values
AOWPathAssetEdModeActor::AOWPathAssetEdModeActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//USplineComponent* spline;
	//spline->CreateSceneProxy()

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	CapsuleComponent->SetCapsuleHalfHeight(88.f);
	CapsuleComponent->SetCapsuleRadius(34.f);
	CapsuleComponent->SetVisibility(true);
	RootComponent = CapsuleComponent;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SphereMesh"));
	StaticMeshComponent->SetupAttachment(GetRootComponent());
	
	//static ConstructorHelpers::FObjectFinder<UStaticMesh>SphereMeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	//static ConstructorHelpers::FObjectFinder<UMaterial>SphereMeshMatterial(TEXT("Material'/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial'"));

	//StaticMeshComponent->SetStaticMesh(SphereMeshAsset.Object);
	//StaticMeshComponent->SetRelativeScale3D(FVector(0.5f));
	//StaticMeshComponent->SetMaterial(0, SphereMeshMatterial.Object);
	

	//StaticMeshComponent->SetSca
	//SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponet"));
	//SphereComponent->SetSphereRadius(34.f);
	//SphereComponent->SetVisibility(true);
	//SphereComponent->SetupAttachment(RootComponent);
}