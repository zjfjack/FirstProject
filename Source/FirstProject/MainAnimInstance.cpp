// Fill out your copyright notice in the Description page of Project Settings.


#include "MainAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MainCharacter.h"

void UMainAnimInstance::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();
    if (Pawn == nullptr)
    {
        Pawn = TryGetPawnOwner();

        if (Pawn)
            MainCharacter = Cast<AMainCharacter>(Pawn);
    }
}

void UMainAnimInstance::UpdateAnimationProperties()
{
    if (Pawn == nullptr)
        Pawn = TryGetPawnOwner();

    if (Pawn)
    {
        FVector Velocity = Pawn->GetVelocity();
        FVector LateralVelocity = FVector(Velocity.X, Velocity.Y, 0.f);
        MovementSpeed = LateralVelocity.Size();

        bIsInAir = Pawn->GetMovementComponent()->IsFalling();

        if (MainCharacter == nullptr)
            MainCharacter = Cast<AMainCharacter>(Pawn);
    }
}