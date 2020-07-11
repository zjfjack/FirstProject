// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MainCharacter.generated.h"

UENUM(BlueprintType)
enum class EMovementStatus : uint8
{
    EMS_Normal UMETA(DisplayName = "Normal"),
    EMS_Sprinting UMETA(DisplayName = "Sprinting"),
    EMS_Dead UMETA(DisplayName = "Dead")
};

UENUM(BlueprintType)
enum class EStaminaStatus : uint8
{
    ESS_Normal UMETA(DisplayName = "Normal"),
    ESS_BelowMinimum UMETA(DisplayName = "BelowMinimum"),
    ESS_Exhausted UMETA(DisplayName = "Exhausted"),
    ESS_ExhaustedRecovering UMETA(DisplayName = "ExhaustedRecovering")
};

UCLASS()
class FIRSTPROJECT_API AMainCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    // Sets default values for this character's properties
    AMainCharacter();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Controller")
    class AMainPlayerController* MainPlayerController = nullptr;

    TArray<FVector> PickUpLocations;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
    class AWeapon* EquippedWeapon = nullptr;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
    class AItem* ActiveOverlappingItem = nullptr;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
    class USpringArmComponent* CameraBoom = nullptr;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
    class UCameraComponent* FollowCamera = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anims")
    class UAnimMontage* CombatMontage = nullptr;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    float BaseTurnRate = 65.f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    float BaseLookUpRate = 65.f;

    bool bSprintKeyIsPressed = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    class UParticleSystem* HitParticles = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    class USoundCue* HitSound = nullptr;

    /**
    /* Player Stats
    */

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player Stats")
    EMovementStatus MovementStatus = EMovementStatus::EMS_Normal;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player Stats")
    EStaminaStatus StaminaStatus = EStaminaStatus::ESS_Normal;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Stats")
    float RunningSpeed = 650.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Stats")
    float SprintingSpeed = 950.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Stats")
    float MaxHealth = 100.f;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Stats")
    float Health = MaxHealth;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Stats")
    float MaxStamina = 350.f;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Stats")
    float Stamina = MaxStamina;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Stats")
    float StaminaDrainRate = 35.f;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Stats")
    float MinSprintStamina = 70.f;

    float InterpSpeed = 15.f;
    float bInterpToEnemy = false;
    void SetInterpToEnemy(bool Interp);

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
    class AEnemy* CombatTarget = nullptr;

    FORCEINLINE void SetCombatTarget(AEnemy* Enemy) { CombatTarget = Enemy; }

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Stats")
    int32 Coins = 0;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player Stats")
    bool bAttacking;

    bool bLMBPressed = false;
    bool bESCPressed = false;

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:	
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    virtual void Jump() override;

    void MoveForward(float Value);
    void MoveRight(float Value);

    void TurnAtRate(float Rate);
    void LookUpAtRate(float Rate);

    void LMBPressed();
    void LMBReleased();

    void ESCPressed();
    void ESCReleased();

    void SprintKeyPressed();
    void SprintKeyReleased();

    FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
    FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

    void SetEquippedWeapon(AWeapon* Weapon);
    FORCEINLINE AWeapon* GetEquippedWeapon() { return EquippedWeapon; }
    FORCEINLINE void SetActiveOverlappingItem(AItem* Item) { ActiveOverlappingItem = Item; }

    UFUNCTION(BlueprintCallable)
    void ShowPickUpLocations();

    void Attack();
    UFUNCTION(BlueprintCallable)
    void AttackEnd();

    void SetMovementStatus(EMovementStatus Status);
    FORCEINLINE void SetStaminaStatus(EStaminaStatus Status) { StaminaStatus = Status; }

    void DecrementHealth(float Amount);

    virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

    UFUNCTION(BlueprintCallable)
    void IncrementCoins(int32 Amount);

    UFUNCTION(BlueprintCallable)
    void IncrementHealth(float Amount);

    void Die();

    UFUNCTION(BlueprintCallable)
    void PlaySwingSound();

    UFUNCTION(BlueprintCallable)
    void DeathEnd();

    void UpdateCombatTarget();
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    TSubclassOf<AEnemy> EnemyFilter;

    void SwitchLevel(FName LevelName);

    UFUNCTION(BlueprintCallable)
    void SaveGame();

    UFUNCTION(BlueprintCallable)
    void LoadGame(bool bSetPosition);

    UFUNCTION(BlueprintImplementableEvent)
    void LoadWeapon(const FString& WeaponName);

private:
    void TickMovementStatus(float DeltaTime);
    void TickInterpingToEnemy(float DeltaTime);
    void TickUpdatingPlayerController();
    FRotator GetLookAtRotationYaw(FVector Target);

    bool bMovingForward = false;
    bool bMovingRight = false;
    bool IsMoving();
};
