// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

UENUM(BlueprintType)
enum class EEnemyMovementStatus : uint8
{
    EMS_Idle UMETA(DisplayName = "Idle"),
    EMS_MoveToTarget UMETA(DisplayName = "MoveToTarget"),
    EMS_Attacking UMETA(DisplayName = "Attacking"),
    EMS_Dead UMETA(DisplayName = "Dead")
};

UCLASS()
class FIRSTPROJECT_API AEnemy : public ACharacter
{
    GENERATED_BODY()

public:
    // Sets default values for this character's properties
    AEnemy();

    bool bHasValidTarget = false;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
    class AAIController* AIController = nullptr;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
    EEnemyMovementStatus MovementStatus = EEnemyMovementStatus::EMS_Idle;

    FORCEINLINE void SetMovementStatus(EEnemyMovementStatus Status) { MovementStatus = Status; }

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
    class USphereComponent* AgroSphere = nullptr;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
    USphereComponent* CombatSphere = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    class UParticleSystem* HitParticles = nullptr;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
    bool bOverlappingCombatSphere = false;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
    AMainCharacter* CombatTarget = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    float MaxHealth = 100.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    float Health = MaxHealth;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    float Damage = 10.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    class USoundCue* HitSound = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    USoundCue* SwingSound = nullptr;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
    class UBoxComponent* CombatCollision = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
    class UAnimMontage* CombatMontage = nullptr;

    FTimerHandle AttackTimer;
    FTimerHandle DeathTimer;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    float DeathDelay = 3.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    float AttackMinTime = .5f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    float AttackMaxTime = 2.f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
    bool bAttacking = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    TSubclassOf<UDamageType> DamageTypeClass;

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:	
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    UFUNCTION()
    virtual void AgroSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
    UFUNCTION()
    virtual void AgroSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
    UFUNCTION()
    virtual void CombatSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
    UFUNCTION()
    virtual void CombatSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
    UFUNCTION()
    void CombatOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
    UFUNCTION()
    void CombatOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    UFUNCTION(BlueprintCallable)
    void MoveToTarget(class AMainCharacter* Target);

    UFUNCTION(BlueprintCallable)
    void ActivateCollision();

    UFUNCTION(BlueprintCallable)
    void DeactivateCollision();

    void Attack();

    UFUNCTION(BlueprintCallable)
    void AttackEnd();

    virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

    void Die();

    UFUNCTION(BlueprintCallable)
    void DeathEnd();

    UFUNCTION(BlueprintCallable)
    bool Alive();

    void Disappear();
};