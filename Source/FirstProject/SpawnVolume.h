// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnVolume.generated.h"

UCLASS()
class FIRSTPROJECT_API ASpawnVolume : public AActor
{
    GENERATED_BODY()
    
public:	
    // Sets default values for this actor's properties
    ASpawnVolume();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
    class UBoxComponent* SpawningBox;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
    TSubclassOf<AActor> Actor1;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
    TSubclassOf<AActor> Actor2;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
    TSubclassOf<AActor> Actor3;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
    TSubclassOf<AActor> Actor4;

    TArray<TSubclassOf<AActor>> SpawnArrary;

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:	
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintPure, Category = "Spawning")
    FVector GetSpawnPoint();

    UFUNCTION(BlueprintPure, Category = "Spawning")
    TSubclassOf<AActor> GetSpawnActor();

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Spawning")
    void SpawnActor(UClass* ToSpawn, const FVector& Location);
};