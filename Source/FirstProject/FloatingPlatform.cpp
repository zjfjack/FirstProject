// Fill out your copyright notice in the Description page of Project Settings.


#include "FloatingPlatform.h"

// Sets default values
AFloatingPlatform::AFloatingPlatform() :
    Mesh(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"))),
    StartPoint(0.f),
    EndPoint(0.f)
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    SetRootComponent(Mesh);
}

// Called when the game starts or when spawned
void AFloatingPlatform::BeginPlay()
{
    Super::BeginPlay();
    
    StartPoint = GetActorLocation();
    // EndPoint is relative location
    EndPoint += StartPoint;
}

// Called every frame
void AFloatingPlatform::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    FVector CurrentLocation = GetActorLocation();
    if ((CurrentLocation - EndPoint).Size() < 1.f)
    {
        // Stop moving
        ToggleIsInterping();
        GetWorldTimerManager().SetTimer(InterpTimer, this, &AFloatingPlatform::ToggleIsInterping, InterpTime);
        Swap(StartPoint, EndPoint);
    }
    
    FVector Interp = FMath::VInterpTo(CurrentLocation, EndPoint, DeltaTime, InterpSpeed);
    SetActorLocation(Interp);
}

void AFloatingPlatform::ToggleIsInterping()
{
    bIsInterping = !bIsInterping;
}