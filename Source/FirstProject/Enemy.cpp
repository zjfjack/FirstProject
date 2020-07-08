// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "AIController.h"
#include "MainCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Sound/SoundCue.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/AnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "MainPlayerController.h"

// Sets default values
AEnemy::AEnemy() :
    AgroSphere(CreateDefaultSubobject<USphereComponent>(TEXT("AgroSphere"))),
    CombatSphere(CreateDefaultSubobject<USphereComponent>(TEXT("CombatSphere"))),
    CombatCollision(CreateDefaultSubobject<UBoxComponent>(TEXT("CombatCollision")))
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    AgroSphere->SetupAttachment(GetRootComponent());
    AgroSphere->InitSphereRadius(600.f);
    AgroSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Ignore);

    CombatSphere->SetupAttachment(GetRootComponent());
    CombatSphere->InitSphereRadius(100.f);

    CombatCollision->SetupAttachment(GetMesh(), "EnemySocket");
    CombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    CombatCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
    CombatCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    CombatCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
    Super::BeginPlay();
    
    AIController = Cast<AAIController>(GetController());

    AgroSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::AgroSphereOnOverlapBegin);
    AgroSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemy::AgroSphereOnOverlapEnd);
    CombatSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::CombatSphereOnOverlapBegin);
    CombatSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemy::CombatSphereOnOverlapEnd);
    CombatCollision->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::CombatOnOverlapBegin);
    CombatCollision->OnComponentEndOverlap.AddDynamic(this, &AEnemy::CombatOnOverlapEnd);
    
    GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
    GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::AgroSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && Alive())
    {
        if (auto Character = Cast<AMainCharacter>(OtherActor))
        {
            MoveToTarget(Character);
        }
    }
}

void AEnemy::AgroSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (OtherActor)
    {
        if (auto Character = Cast<AMainCharacter>(OtherActor))
        {
            bHasValidTarget = false;
            if (Character->CombatTarget == this)
            {
                Character->SetCombatTarget(nullptr);
            }
            Character->UpdateCombatTarget();
            SetMovementStatus(EEnemyMovementStatus::EMS_Idle);
            if (AIController)
            {
                AIController->StopMovement();
            }
        }
    }
}

void AEnemy::CombatSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) 
{
    
    if (OtherActor && Alive())
    {
        if (auto Character = Cast<AMainCharacter>(OtherActor))
        {
            bHasValidTarget = true;
            //Character->SetCombatTarget(this);
            Character->UpdateCombatTarget();
            CombatTarget = Character;
            bOverlappingCombatSphere = true;
            Attack();
        }
    }
}

void AEnemy::CombatSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) 
{
    if (OtherActor && Alive() && OtherComp)
    {
        if (auto Character = Cast<AMainCharacter>(OtherActor))
        {
            bOverlappingCombatSphere = false;
            MoveToTarget(Character);
            CombatTarget = nullptr;

            if (Character->CombatTarget == this)
            {
                Character->SetCombatTarget(nullptr);
                Character->UpdateCombatTarget();
            }
            if (Character->MainPlayerController)
            {
                if (auto CharacterMesh = Cast<USkeletalMeshComponent>(OtherComp))
                    Character->MainPlayerController->HideEnemyHealthBar();
            }
            GetWorldTimerManager().ClearTimer(AttackTimer);
        }
    }
}

void AEnemy::CombatOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor)
    {
        if (auto Character = Cast<AMainCharacter>(OtherActor))
        {
            if (auto CharacterHitParticles = Character->HitParticles)
            {
                const USkeletalMeshSocket* TipSocket = GetMesh()->GetSocketByName("TipSocket");
                if (TipSocket)
                {
                    FVector SocketLocation = TipSocket->GetSocketLocation(GetMesh());
                    UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), CharacterHitParticles, SocketLocation, FRotator::ZeroRotator, false);
                }
            }
            if (auto CharacterHitSound = Character->HitSound)
                UGameplayStatics::PlaySound2D(this, CharacterHitSound);

            if (DamageTypeClass)
                UGameplayStatics::ApplyDamage(Character, Damage, AIController, this, DamageTypeClass);
        }
    }
}

void AEnemy::CombatOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

void AEnemy::ActivateCollision()
{
    CombatCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    if (SwingSound)
        UGameplayStatics::PlaySound2D(this, SwingSound);
}

void AEnemy::DeactivateCollision()
{
    CombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AEnemy::Attack()
{
    if (!Alive() || !bHasValidTarget) return;
    if (AIController)
    {
        AIController->StopMovement();
        SetMovementStatus(EEnemyMovementStatus::EMS_Attacking);
    }

    if (!bAttacking)
    {
        bAttacking = true;
        if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
        {
            AnimInstance->Montage_Play(CombatMontage, 1.35f);
            AnimInstance->Montage_JumpToSection("Attack", CombatMontage);
        }
    }
}

void AEnemy::AttackEnd()
{
    bAttacking = false;
    if (bOverlappingCombatSphere)
    {
        float AttackTime = FMath::FRandRange(AttackMinTime, AttackMaxTime);
        GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemy::Attack, AttackTime);
    }
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    Health -= DamageAmount;
    if (Health <= 0.f)
        Die(DamageCauser);
    return DamageAmount;
}

void AEnemy::Die(AActor* Causer)
{
    if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
    {
        AnimInstance->Montage_Play(CombatMontage);
        AnimInstance->Montage_JumpToSection("Death", CombatMontage);
    }
    SetMovementStatus(EEnemyMovementStatus::EMS_Dead);
    CombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    AgroSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    CombatSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    AIController->StopMovement();

    if (Causer)
    {
        if (auto Character = Cast<AMainCharacter>(Causer))
        {
            Character->UpdateCombatTarget();
        }
    }
}

void AEnemy::DeathEnd()
{
    UE_LOG(LogTemp, Warning, TEXT("Death End"));
    GetMesh()->bPauseAnims = true;
    GetMesh()->bNoSkeletonUpdate = true;

    GetWorldTimerManager().SetTimer(DeathTimer, this, &AEnemy::Disappear, DeathDelay);
}

void AEnemy::MoveToTarget(AMainCharacter* Target)
{
    UE_LOG(LogTemp, Warning, TEXT("MoveToTarget"));
    SetMovementStatus(EEnemyMovementStatus::EMS_MoveToTarget);
    if (AIController)
    {
        FAIMoveRequest MoveRequest(Target);
        MoveRequest.SetAcceptanceRadius(25.f);

        FNavPathSharedPtr NavPath;
        AIController->MoveTo(MoveRequest, &NavPath);
        //TArray<FNavPathPoint> PathPoints = NavPath->GetPathPoints();
        //for (auto Point : PathPoints)
        //{
        //	UKismetSystemLibrary::DrawDebugSphere(this, Point.Location, 25.f, 12, FLinearColor::Blue, 5.f, 2.f);
        //}

        //AIController->MoveToActor(Target);
    }
}

bool AEnemy::Alive()
{
    return MovementStatus != EEnemyMovementStatus::EMS_Dead;
}

void AEnemy::Disappear()
{
    Destroy();
}