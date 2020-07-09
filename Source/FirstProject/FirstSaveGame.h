// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "FCharacterStats.h"
#include "FirstSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class FIRSTPROJECT_API UFirstSaveGame : public USaveGame
{
    GENERATED_BODY()
    
public:

    UFirstSaveGame();

    UPROPERTY(VisibleAnywhere, Category = "Basic")
    FString PlayerName = "Default";

    UPROPERTY(VisibleAnywhere, Category = "Basic")
    uint32 UserIndex = 0;

    UPROPERTY(VisibleAnywhere, Category = "Basic")
    struct FCharacterStats CharacterStats;
};