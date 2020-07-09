// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnVolume.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Enemy.h"
#include "AIController.h"

// Sets default values
ASpawnVolume::ASpawnVolume() :
    SpawningBox(CreateDefaultSubobject<UBoxComponent>(TEXT("SpawningBox")))
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    SetRootComponent(SpawningBox);
}

// Called when the game starts or when spawned
void ASpawnVolume::BeginPlay()
{
    Super::BeginPlay();
    
    if (Actor1 && Actor2 && Actor3 && Actor4)
    {
        SpawnArrary = { Actor1, Actor2, Actor3, Actor4 };
    }
}

// Called every frame
void ASpawnVolume::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

FVector ASpawnVolume::GetSpawnPoint()
{
    FVector Extent = SpawningBox->GetScaledBoxExtent();
    FVector Origin = SpawningBox->GetComponentLocation();

    FVector SpawningPoint = UKismetMathLibrary::RandomPointInBoundingBox(Origin, Extent);
    return SpawningPoint;
}

TSubclassOf<AActor> ASpawnVolume::GetSpawnActor()
{
    if (SpawnArrary.Num() > 0)
    {
        int32 Selection = FMath::RandRange(0, SpawnArrary.Num() - 1);
        return SpawnArrary[Selection];
    }
    return nullptr;
}

void ASpawnVolume::SpawnActor_Implementation(UClass* ToSpawn, const FVector& Location)
{
    if (ToSpawn)
    {
        UWorld* World = GetWorld();
        if (World)
        {
            AActor* Actor = World->SpawnActor<AActor>(ToSpawn, Location, FRotator(0.f));
            if (auto Enemy = Cast<AEnemy>(Actor))
            {
                Enemy->SpawnDefaultController();
                if (AAIController* AIController = Cast<AAIController>(Enemy->GetController()))
                    Enemy->AIController = AIController;
            }
        }
    }
}