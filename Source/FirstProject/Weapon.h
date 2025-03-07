// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Weapon.generated.h"

UENUM(BlueprintType)
enum class EWeaponState : uint8
{
    EWS_Pickup UMETA(DisplayName = "Pickup"),
    EWS_Equipped UMETA(DisplayName = "Equipped")
};

/**
 * 
 */
UCLASS()
class FIRSTPROJECT_API AWeapon : public AItem
{
    GENERATED_BODY()

public:

    AWeapon();

    UPROPERTY(EditDefaultsOnly, Category = "SavedData")
    FString Name;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item")
    EWeaponState WeaponState = EWeaponState::EWS_Pickup;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Particles")
    bool bWeaponParticle = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Sound")
    class USoundCue* OnEquipSound = nullptr;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SkeletalMesh")
    class USkeletalMeshComponent* SkeletalMesh = nullptr;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item | Combat")
    class UBoxComponent* CombatCollision = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Combat")
    float Damage = 25.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Sound")
    USoundCue* SwingSound = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Combat")
    TSubclassOf<UDamageType> DamageTypeClass;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item | Combat")
    AController* WeaponInstigator = nullptr;

    FORCEINLINE void SetWeaponInstigator(AController* Inst) { WeaponInstigator = Inst; }

protected:

    virtual void BeginPlay() override;

public:
    
    virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
    virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

    UFUNCTION(BlueprintCallable)
    void Equip(class AMainCharacter* Character);

    FORCEINLINE void SetWeaponState(EWeaponState State) { WeaponState = State; }
    FORCEINLINE EWeaponState GetWeaponState() { return WeaponState; }

    UFUNCTION()
    void CombatOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
    UFUNCTION()
    void CombatOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    UFUNCTION(BlueprintCallable)
    void ActivateCollision();

    UFUNCTION(BlueprintCallable)
    void DeactivateCollision();
};