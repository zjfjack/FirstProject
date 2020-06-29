// Fill out your copyright notice in the Description page of Project Settings.


#include "FloorSwitchActor.h"
#include "Components/BoxComponent.h"

// Sets default values
AFloorSwitchActor::AFloorSwitchActor() :
    TriggerBox(CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"))),
    FloorSwitch(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FloorSwitch"))),
    Door(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door")))
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    
    SetRootComponent(TriggerBox);

    TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    TriggerBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
    TriggerBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    TriggerBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

    TriggerBox->SetBoxExtent(FVector(62.f, 62.f, 32.f));

    FloorSwitch->SetupAttachment(GetRootComponent());
    Door->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void AFloorSwitchActor::BeginPlay()
{
    Super::BeginPlay();
    
    TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AFloorSwitchActor::OnOverlapBegin);
    TriggerBox->OnComponentEndOverlap.AddDynamic(this, &AFloorSwitchActor::OnOverlapEnd);

    InitialDoorLocation = Door->GetComponentLocation();
    InitialSwitchLocation = FloorSwitch->GetComponentLocation();
}

// Called every frame
void AFloorSwitchActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

void AFloorSwitchActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    bCharacterOnSwitch = true;
    RaiseDoor();
    LowerFloorSwitch();
}

void AFloorSwitchActor::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    bCharacterOnSwitch = false;
    GetWorldTimerManager().SetTimer(SwitchHandle, this, &AFloorSwitchActor::CloseDoor, SwitchTime);
}

void AFloorSwitchActor::UpdateDoorLocation(float Z)
{
    FVector NewLocation = InitialDoorLocation;
    NewLocation.Z += Z;
    Door->SetWorldLocation(NewLocation);
}

void AFloorSwitchActor::UpdateFloorSwitch(float Z)
{
    FVector NewLocation = InitialSwitchLocation;
    NewLocation.Z += Z;
    FloorSwitch->SetWorldLocation(NewLocation);
}

void AFloorSwitchActor::CloseDoor()
{
    if (!bCharacterOnSwitch)
    {
        LowerDoor();
        RaiseFloorSwitch();
    }
}