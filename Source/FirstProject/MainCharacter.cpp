// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

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

void AMainCharacter::IncrementCoins(int32 Amount)
{
	Coins += Amount;
}

void AMainCharacter::Die()
{
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float DeltaStamina = StaminaDrainRate * DeltaTime;

	switch (StaminaStatus)
	{
	case EStaminaStatus::ESS_Normal:
		if (bSprintKeyIsPressed)
		{
			Stamina -= DeltaStamina;
			if (Stamina <= MinSprintStamina)
				StaminaStatus = EStaminaStatus::ESS_BelowMinimum;
			SetMovementStatus(EMovementStatus::EMS_Sprinting);
		}
		else
		{
			Stamina = FMath::Min(Stamina + DeltaStamina, MaxStamina);
			SetMovementStatus(EMovementStatus::EMS_Normal);
		}
		break;
	case EStaminaStatus::ESS_BelowMinimum:
		if (bSprintKeyIsPressed)
		{
			Stamina = FMath::Max(Stamina - DeltaStamina, 0.f);
			if (FMath::IsNearlyZero(Stamina))
			{
				SetStaminaStatus(EStaminaStatus::ESS_Exhausted);
				SetMovementStatus(EMovementStatus::EMS_Normal);
			}
			else
				SetMovementStatus(EMovementStatus::EMS_Sprinting);
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
		if (!bSprintKeyIsPressed)
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

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AMainCharacter::SprintKeyPressed);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AMainCharacter::SprintKeyReleased);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMainCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMainCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AMainCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AMainCharacter::LookUpAtRate);
}

void AMainCharacter::MoveForward(float Value)
{
	if (Controller != nullptr && Value != 0.f)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AMainCharacter::MoveRight(float Value)
{
	if (Controller != nullptr && Value != 0.f)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
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

void AMainCharacter::SprintKeyPressed()
{
	bSprintKeyIsPressed = true;
}

void AMainCharacter::SprintKeyReleased()
{
	bSprintKeyIsPressed = false;
}