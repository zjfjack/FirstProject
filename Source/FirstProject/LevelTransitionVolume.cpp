// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelTransitionVolume.h"
#include "Components/BoxComponent.h"
#include "Components/BillboardComponent.h"
#include "MainCharacter.h"

// Sets default values
ALevelTransitionVolume::ALevelTransitionVolume() :
    TransitionVolume(CreateDefaultSubobject<UBoxComponent>(TEXT("Transition"))),
    Billboard(CreateDefaultSubobject<UBillboardComponent>(TEXT("Billboard")))
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;

    SetRootComponent(TransitionVolume);
    Billboard->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void ALevelTransitionVolume::BeginPlay()
{
    Super::BeginPlay();

    TransitionVolume->OnComponentBeginOverlap.AddDynamic(this, &ALevelTransitionVolume::OnOverlapBegin);
}

// Called every frame
void ALevelTransitionVolume::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

void ALevelTransitionVolume::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor)
    {
        if (auto Character = Cast<AMainCharacter>(OtherActor))
            Character->SwitchLevel(TransitionLevelName);
    }
}