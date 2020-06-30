// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "MainCharacter.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

AWeapon::AWeapon() :
    SkeletalMesh(CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh")))
{
    SkeletalMesh->SetupAttachment(GetRootComponent());
}

void AWeapon::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
    
    if (WeaponState == EWeaponState::EWS_Pickup && OtherActor)
    {
        if (auto Character = Cast<AMainCharacter>(OtherActor))
            Character->SetActiveOverlappingItem(this);
    }
}

void AWeapon::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

    if (OtherActor)
    {
        if (auto Character = Cast<AMainCharacter>(OtherActor))
            Character->SetActiveOverlappingItem(nullptr);
    }
}

void AWeapon::Equip(AMainCharacter* Character)
{
    if (Character)
    {
        //SkeletalMesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
        //SkeletalMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
        //SkeletalMesh->SetSimulatePhysics(false);

        const USkeletalMeshSocket* RightHandSocket = Character->GetMesh()->GetSocketByName("RightHandSocket");
        if (RightHandSocket)
        {
            bRotate = false;
            RightHandSocket->AttachActor(this, Character->GetMesh());
            Character->SetEquippedWeapon(this);
            Character->SetActiveOverlappingItem(nullptr);

            if (OnEquipSound)
                UGameplayStatics::PlaySound2D(this, OnEquipSound);
            if (!bWeaponParticle)
                IdleParticleComponent->Deactivate();
        }
    }
}