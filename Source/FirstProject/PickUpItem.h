// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "PickUpItem.generated.h"

/**
 * 
 */
UCLASS()
class FIRSTPROJECT_API APickUpItem : public AItem
{
    GENERATED_BODY()

public:
    APickUpItem();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Coins")
    int32 CoinCount = 1;

    virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
    virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
};