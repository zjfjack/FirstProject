// Fill out your copyright notice in the Description page of Project Settings.


#include "ExplosiveItem.h"
#include "MainCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Enemy.h"

AExplosiveItem::AExplosiveItem()
{

}

void AExplosiveItem::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

    if (OtherActor)
    {
        auto Character = Cast<AMainCharacter>(OtherActor);
        auto Enemy = Cast<AEnemy>(OtherActor);
        if (Character || Enemy)
        {
            if (OverlapParticles)
                UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OverlapParticles, GetActorLocation(), FRotator(0.f));
            if (OverlapSound)
                UGameplayStatics::PlaySound2D(this, OverlapSound);

            UGameplayStatics::ApplyDamage(OtherActor, Damage, nullptr, this, DamageTypeClass);
            Destroy();
        }

    }
}

void AExplosiveItem::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}