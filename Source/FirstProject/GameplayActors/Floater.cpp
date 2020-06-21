// Fill out your copyright notice in the Description page of Project Settings.


#include "Floater.h"

// Sets default values
AFloater::AFloater() :
	StaticMesh(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CustomStaticMesh"))),
	InitialLocation(FVector(0.0f)), 
	PlacedLocation(FVector(0.0f)),  
	WorldOrigin(FVector(0.0f)),
	InitialDirection(FVector(0.0f)),
	bShouldFloat(false),
	bInitializedFloaterLocations(false)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetRootComponent(StaticMesh);
}

// Called when the game starts or when spawned
void AFloater::BeginPlay()
{
	Super::BeginPlay();
	
	PlacedLocation = GetActorLocation();

	if (bInitializedFloaterLocations)
		SetActorLocation(InitialLocation);
}

// Called every frame
void AFloater::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bShouldFloat)
	{
		FHitResult HitResult;
		AddActorLocalOffset(InitialDirection, false, &HitResult);
	}
}

