// Fill out your copyright notice in the Description page of Project Settings.


#include "FCharacterStats.h"

FCharacterStats::FCharacterStats(float Health, float MaxHealth, float Stamina, float MaxStamina, int32 Coins, FVector Location, FRotator Rotation) :
    Health(Health), MaxHealth(MaxHealth), Stamina(Stamina), MaxStamina(MaxStamina), Coins(Coins), 
    Location(MoveTemp(Location)), Rotation(MoveTemp(Rotation))
{
}