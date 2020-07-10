// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "MainCharacter.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Enemy.h"

AWeapon::AWeapon() :
    SkeletalMesh(CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"))),
    CombatCollision(CreateDefaultSubobject<UBoxComponent>(TEXT("CombatCollision")))
{
    SkeletalMesh->SetupAttachment(GetRootComponent());
    CombatCollision->SetupAttachment(GetRootComponent());

    CombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    CombatCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
    CombatCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    CombatCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
}

void AWeapon::BeginPlay()
{
    Super::BeginPlay();

    CombatCollision->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::CombatOnOverlapBegin);
    CombatCollision->OnComponentEndOverlap.AddDynamic(this, &AWeapon::CombatOnOverlapEnd);
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
        SkeletalMesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
        SkeletalMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
        SkeletalMesh->SetSimulatePhysics(false);

        SetWeaponInstigator(Character->GetController());

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

void AWeapon::CombatOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor)
    {
        if (auto Enemy = Cast<AEnemy>(OtherActor))
        {
            if (auto HitParticles = Enemy->HitParticles)
            {
                const USkeletalMeshSocket* WeaponSocket = SkeletalMesh->GetSocketByName("WeaponSocket");
                if (WeaponSocket)
                {
                    FVector SocketLocation = WeaponSocket->GetSocketLocation(SkeletalMesh);
                    UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticles, SocketLocation, FRotator::ZeroRotator, false);
                }
            }
            if (auto HitSound = Enemy->HitSound)
                UGameplayStatics::PlaySound2D(this, HitSound);
            if (DamageTypeClass && WeaponInstigator)
                UGameplayStatics::ApplyDamage(Enemy, Damage, WeaponInstigator, this, DamageTypeClass);
        }
    }
}

void AWeapon::CombatOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

void AWeapon::ActivateCollision()
{
    CombatCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AWeapon::DeactivateCollision()
{
    CombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}