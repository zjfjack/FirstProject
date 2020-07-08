// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUpItem.h"
#include "MainCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

APickUpItem::APickUpItem()
{
}

void APickUpItem::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

    if (OtherActor)
    {
        if (auto Character = Cast<AMainCharacter>(OtherActor))
        {
            OnPickUpAction(Character);
            Character->PickUpLocations.Add(GetActorLocation());

            if (OverlapParticles)
                UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OverlapParticles, GetActorLocation(), FRotator(0.f));
            if (OverlapSound)
                UGameplayStatics::PlaySound2D(this, OverlapSound);

            Destroy();
        }
    }
}

void APickUpItem::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}