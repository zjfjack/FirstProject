// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FCharacterStats.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FCharacterStats
{
    GENERATED_BODY()

    FCharacterStats() = default;
    FCharacterStats(float Health, float MaxHealth, float Stamina, float MaxStamina, int32 Coins, FVector Location, FRotator Rotation, FString WeaponName, FString LevelName);

    UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
    float Health;

    UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
    float MaxHealth;

    UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
    float Stamina;

    UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
    float MaxStamina;

    UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
    int32 Coins;

    UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
    FVector Location;

    UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
    FRotator Rotation;

    UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
    FString WeaponName;

    UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
    FString LevelName;
};