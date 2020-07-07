// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Weapon.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Enemy.h"
#include "MainPlayerController.h"

// Sets default values
AMainCharacter::AMainCharacter() :
    CameraBoom(CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"))),
    FollowCamera(CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera")))
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    CameraBoom->SetupAttachment(GetRootComponent());
    CameraBoom->TargetArmLength = 300.f;

    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

    // Disable character rotate with camera
    bUseControllerRotationYaw = false;

    // configure character movement
    auto Movement = GetCharacterMovement();
    Movement->bOrientRotationToMovement = true; // character moves in the direction of input
    Movement->RotationRate = FRotator(0.f, 540.f, 0.f); // at this rotation rate
    Movement->JumpZVelocity = 650.f;
    Movement->AirControl = 0.2f;
}

void AMainCharacter::ShowPickUpLocations()
{
    for (auto PickUpLocation : PickUpLocations)
        UKismetSystemLibrary::DrawDebugSphere(this, PickUpLocation, 25.f, 12, FLinearColor::Red, 5.f, 2.f);
}

void AMainCharacter::SetMovementStatus(EMovementStatus Status)
{
    MovementStatus = Status;
    GetCharacterMovement()->MaxWalkSpeed = Status == EMovementStatus::EMS_Normal ? RunningSpeed : SprintingSpeed;
}

void AMainCharacter::DecrementHealth(float Amount)
{
    Health -= Amount;
    if (Health <= 0)
        Die();
}

float AMainCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    Health -= DamageAmount;
    if (Health <= 0)
    {
        Die();
        if (auto Enemy = Cast<AEnemy>(DamageCauser))
        {
            Enemy->bHasValidTarget = false;
        }
    }
        
    return DamageAmount;
}

void AMainCharacter::IncrementCoins(int32 Amount)
{
    Coins += Amount;
}

void AMainCharacter::Die()
{
    if (MovementStatus == EMovementStatus::EMS_Dead) return;
    UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
    if (AnimInstance && CombatMontage)
    {
        AnimInstance->Montage_Play(CombatMontage);
        AnimInstance->Montage_JumpToSection("Death", CombatMontage);
    }
    SetMovementStatus(EMovementStatus::EMS_Dead);
}

void AMainCharacter::DeathEnd()
{
    GetMesh()->bPauseAnims = true;
    GetMesh()->bNoSkeletonUpdate = true;
}

void AMainCharacter::PlaySwingSound()
{
    if (auto SwingSound = EquippedWeapon->SwingSound)
        UGameplayStatics::PlaySound2D(this, SwingSound);
}

void AMainCharacter::SetInterpToEnemy(bool Interp)
{
    bInterpToEnemy = Interp;
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
    Super::BeginPlay();

    MainPlayerController = Cast<AMainPlayerController>(GetController());
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (MovementStatus == EMovementStatus::EMS_Dead) return;

    TickMovementStatus(DeltaTime);
    TickInterpingToEnemy(DeltaTime);
    TickUpdatingPlayerController();
}

void AMainCharacter::TickMovementStatus(float DeltaTime)
{
    float DeltaStamina = StaminaDrainRate * DeltaTime;
    switch (StaminaStatus)
    {
    case EStaminaStatus::ESS_Normal:
        if (bSprintKeyIsPressed && IsMoving())
        {
            Stamina -= DeltaStamina;
            if (Stamina <= MinSprintStamina)
                StaminaStatus = EStaminaStatus::ESS_BelowMinimum;
            SetMovementStatus(IsMoving() ? EMovementStatus::EMS_Sprinting : EMovementStatus::EMS_Normal);
        }
        else
        {
            Stamina = FMath::Min(Stamina + DeltaStamina, MaxStamina);
            SetMovementStatus(EMovementStatus::EMS_Normal);
        }
        break;
    case EStaminaStatus::ESS_BelowMinimum:
        if (bSprintKeyIsPressed && IsMoving())
        {
            if (!IsMoving()) return;
            Stamina = FMath::Max(Stamina - DeltaStamina, 0.f);
            if (FMath::IsNearlyZero(Stamina))
            {
                SetStaminaStatus(EStaminaStatus::ESS_Exhausted);
                SetMovementStatus(EMovementStatus::EMS_Normal);
            }
            else
                SetMovementStatus(IsMoving() ? EMovementStatus::EMS_Sprinting : EMovementStatus::EMS_Normal);
        }
        else
        {
            Stamina += DeltaStamina;
            if (Stamina >= MinSprintStamina)
                SetStaminaStatus(EStaminaStatus::ESS_Normal);
            SetMovementStatus(EMovementStatus::EMS_Normal);
        }
        break;
    case EStaminaStatus::ESS_Exhausted:
        if (!bSprintKeyIsPressed || !IsMoving())
        {
            Stamina += DeltaStamina;
            SetStaminaStatus(EStaminaStatus::ESS_ExhaustedRecovering);
        }
        break;
    case EStaminaStatus::ESS_ExhaustedRecovering:
        Stamina += DeltaStamina;
        if (Stamina >= MinSprintStamina)
            SetStaminaStatus(EStaminaStatus::ESS_Normal);
        break;
    }
}

void AMainCharacter::TickInterpingToEnemy(float DeltaTime)
{
    if (bInterpToEnemy && CombatTarget)
    {
        FRotator LookAtYaw = GetLookAtRotationYaw(CombatTarget->GetActorLocation());
        FRotator InterpRotation = FMath::RInterpTo(GetActorRotation(), LookAtYaw, DeltaTime, InterpSpeed);
        SetActorRotation(InterpRotation);
    }
}

void AMainCharacter::TickUpdatingPlayerController()
{
    if (CombatTarget && MainPlayerController)
    {
        MainPlayerController->EnemyLocation = CombatTarget->GetActorLocation();
    }
}

FRotator AMainCharacter::GetLookAtRotationYaw(FVector Target)
{
    FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Target);
    FRotator LookAtRotationYaw(0.f, LookAtRotation.Yaw, 0.f);
    return LookAtRotationYaw;
}

bool AMainCharacter::IsMoving()
{
    return bMovingForward || bMovingRight;
}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    check(PlayerInputComponent);

    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMainCharacter::Jump);
    PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
    PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AMainCharacter::SprintKeyPressed);
    PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AMainCharacter::SprintKeyReleased);
    PlayerInputComponent->BindAction("LMB", IE_Pressed, this, &AMainCharacter::LMBPressed);
    PlayerInputComponent->BindAction("LMB", IE_Released, this, &AMainCharacter::LMBReleased);

    PlayerInputComponent->BindAxis("MoveForward", this, &AMainCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &AMainCharacter::MoveRight);

    PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
    PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
    PlayerInputComponent->BindAxis("TurnRate", this, &AMainCharacter::TurnAtRate);
    PlayerInputComponent->BindAxis("LookUpRate", this, &AMainCharacter::LookUpAtRate);
}

void AMainCharacter::Jump()
{
    if (MovementStatus != EMovementStatus::EMS_Dead)
        ACharacter::Jump();
}

void AMainCharacter::MoveForward(float Value)
{
    bMovingForward = false;
    if (Controller != nullptr && Value != 0.f && !bAttacking && MovementStatus != EMovementStatus::EMS_Dead)
    {
        // find out which way is forward
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        AddMovementInput(Direction, Value);

        bMovingForward = true;
    }
}

void AMainCharacter::MoveRight(float Value)
{
    bMovingRight = false;
    if (Controller != nullptr && Value != 0.f && !bAttacking && MovementStatus != EMovementStatus::EMS_Dead)
    {
        // find out which way is forward
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
        AddMovementInput(Direction, Value);

        bMovingRight = true;
    }
}

void AMainCharacter::TurnAtRate(float Rate)
{
    AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AMainCharacter::LookUpAtRate(float Rate)
{
    AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AMainCharacter::LMBPressed()
{
    bLMBPressed = true;

    if (MovementStatus == EMovementStatus::EMS_Dead) return;

    if (ActiveOverlappingItem)
    {
        if (auto Weapon = Cast<AWeapon>(ActiveOverlappingItem))
        {
            Weapon->Equip(this);
            SetActiveOverlappingItem(nullptr);
        }
    }
    else if (GetEquippedWeapon())
        Attack();
}

void AMainCharacter::LMBReleased()
{
    bLMBPressed = false;
}

void AMainCharacter::SprintKeyPressed()
{
    bSprintKeyIsPressed = true;
}

void AMainCharacter::SprintKeyReleased()
{
    bSprintKeyIsPressed = false;
}

void AMainCharacter::SetEquippedWeapon(AWeapon* Weapon)
{
    if (EquippedWeapon)
        EquippedWeapon->Destroy();
    EquippedWeapon = Weapon;
}

void AMainCharacter::Attack()
{
    if (bAttacking || MovementStatus == EMovementStatus::EMS_Dead) return;

    bAttacking = true;
    SetInterpToEnemy(true);
    UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
    if (AnimInstance && CombatMontage)
    {
        int32 Section = FMath::RandRange(0, 1);
        AnimInstance->Montage_Play(CombatMontage, 2.f);
        FName SectionName = Section == 0 ? "Attack_1" : "Attack_2";
        AnimInstance->Montage_JumpToSection(SectionName, CombatMontage);
    }
}

void AMainCharacter::AttackEnd()
{
    bAttacking = false;
    SetInterpToEnemy(false);
    if (bLMBPressed)
        Attack();		
}