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
	bInitializedFloaterLocations(false),
	InitialForce(FVector(20000000.f, 0.f, 0.f)),
	InitialTorque(FVector(20000000.f, 0.f, 0.f)),
	RunningTime(0.f),
	BaseZLocation(0.f),
	Amplitude(0.f),
	Period(0.f),
	PhaseShift(0.f),
	VerticalShift(0.f)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetRootComponent(StaticMesh);
}

// Called when the game starts or when spawned
void AFloater::BeginPlay()
{
	Super::BeginPlay();

	InitialLocation = FVector(FMath::FRandRange(-500.f, 500.f), FMath::FRandRange(-500.f, 500.f), FMath::FRandRange(0.f, 500.f));
	
	PlacedLocation = GetActorLocation();

	if (bInitializedFloaterLocations)
		SetActorLocation(InitialLocation);

	BaseZLocation = PlacedLocation.Z;

	// FHitResult HitResult;
	// FVector LocalOffset = FVector(500.f, 0.f, 0.f);

	// StaticMesh->AddForce(InitialForce);
	// StaticMesh->AddTorque(InitialTorque);

	// AddActorLocalOffset(LocalOffset, true, &HitResult);
	// AddActorWorldOffset(LocalOffset, true, &HitResult);
}

// Called every frame
void AFloater::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bShouldFloat)
	{
		/*FHitResult HitResult;
		AddActorLocalOffset(InitialDirection, true, &HitResult);*/
		FVector NewLocation = GetActorLocation();

		// NewLocation.Z = BaseZLocation + 100.f * FMath::Sin(RunningTime);
		NewLocation.Z = BaseZLocation + Amplitude * FMath::Sin(Period * RunningTime - PhaseShift) + VerticalShift;

		SetActorLocation(NewLocation);
		RunningTime += DeltaTime;
	}

	// FRotator Rotation = FRotator(0.0f, 0.0f, 1.f);
	// AddActorLocalRotation(Rotation);
	// AddActorWorldRotation(Rotation);
}

