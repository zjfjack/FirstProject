// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class FIRSTPROJECT_API AMainPlayerController : public APlayerController
{
    GENERATED_BODY()

public:

    /** Reference to the UMG asset in the editor */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
    TSubclassOf<class UUserWidget> WHUDOverlay;
    
    /** Variable to hold the widget after creating it*/
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Widgets")
    UUserWidget* HUDOverlay = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
    TSubclassOf<UUserWidget> WEnemyHealthBar;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Widgets")
    UUserWidget* EnemyHealthBar = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
    TSubclassOf<UUserWidget> WPauseMenu;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Widgets")
    UUserWidget* PauseMenu = nullptr;

    bool bPauseMenuVisible = false;
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
    void DisplayPauseMenu();
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
    void HidePauseMenu();
    void TogglePauseMenu();

    bool bEnenmyHealthBarVisible = false;
    void DisplayEnemyHealthBar();
    void HideEnemyHealthBar();

    FVector EnemyLocation;

protected:

    void BeginPlay() override;

    virtual void Tick(float DeltaTime) override;
};