// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

UCLASS()
class FIRSTPROJECT_API AItem : public AActor
{
    GENERATED_BODY()
    
public:	
    // Sets default values for this actor's properties
    AItem();

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item | Collision")
    class USphereComponent* CollisionVolume;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item | Mesh")
    UStaticMeshComponent* Mesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Particles")
    UParticleSystemComponent* IdleParticleComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Particles")
    UParticleSystem* OverlapParticles;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Sounds")
    class USoundCue* OverlapSound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | ItemProperties")
    bool bRotate = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | ItemProperties")
    float rotationRate = 45.f;

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:	
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    UFUNCTION()
    virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
    UFUNCTION()
    virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};