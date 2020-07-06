// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

// Sets default values
AItem::AItem() :
    CollisionVolume(CreateDefaultSubobject<USphereComponent>(TEXT("CollisionVolume"))),
    Mesh(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"))),
    IdleParticleComponent(CreateDefaultSubobject<UParticleSystemComponent>(TEXT("IdleParticleComponent")))
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    SetRootComponent(CollisionVolume);

    CollisionVolume->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnOverlapBegin);
    CollisionVolume->OnComponentEndOverlap.AddDynamic(this, &AItem::OnOverlapEnd);

    Mesh->SetupAttachment(GetRootComponent());
    IdleParticleComponent->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
    Super::BeginPlay();
    
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bRotate)
    {
        FRotator Rotation = GetActorRotation();
        Rotation.Yaw += DeltaTime * rotationRate;
        SetActorRotation(Rotation);
    }
}

void AItem::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

    if (OverlapParticles)
    {
        UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OverlapParticles, GetActorLocation(), FRotator(0.f));
    }
    if (OverlapSound)
    {
        UGameplayStatics::PlaySound2D(this, OverlapSound);
    }
}

void AItem::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}