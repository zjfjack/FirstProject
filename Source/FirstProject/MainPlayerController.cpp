// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"
#include "Blueprint/UserWidget.h"

void AMainPlayerController::BeginPlay()
{
    Super::BeginPlay();

    if (WHUDOverlay)
        HUDOverlay = CreateWidget<UUserWidget>(this, WHUDOverlay);
    HUDOverlay->AddToViewport();
    HUDOverlay->SetVisibility(ESlateVisibility::Visible);

    if (WEnemyHealthBar)
    {
        EnemyHealthBar = CreateWidget<UUserWidget>(this, WEnemyHealthBar);
        if (EnemyHealthBar)
        {
            EnemyHealthBar->AddToViewport();
            EnemyHealthBar->SetVisibility(ESlateVisibility::Hidden);
        }
        EnemyHealthBar->SetAlignmentInViewport(FVector2D::ZeroVector);
    }

    if (WPauseMenu)
    {
        PauseMenu = CreateWidget<UUserWidget>(this, WPauseMenu);
        if (PauseMenu)
        {
            PauseMenu->AddToViewport();
            PauseMenu->SetVisibility(ESlateVisibility::Hidden);
        }
    }
}

void AMainPlayerController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (EnemyHealthBar)
    {
        FVector2D PositionInViewport;
        ProjectWorldLocationToScreen(EnemyLocation, PositionInViewport);
        PositionInViewport.Y -= 85.f;

        FVector2D SizeInViewport(200.f, 25.f);
        EnemyHealthBar->SetPositionInViewport(PositionInViewport);
        EnemyHealthBar->SetDesiredSizeInViewport(SizeInViewport);
    }
}

void AMainPlayerController::DisplayPauseMenu_Implementation()
{
    if (PauseMenu)
    {
        bPauseMenuVisible = true;
        PauseMenu->SetVisibility(ESlateVisibility::Visible);
        FInputModeGameAndUI InputModeGameAndUI;
        SetInputMode(InputModeGameAndUI);
        bShowMouseCursor = true;
    }
}

void AMainPlayerController::HidePauseMenu_Implementation()
{
    if (PauseMenu)
    {
        FInputModeGameOnly InputModeGameOnly;
        SetInputMode(InputModeGameOnly);
        bShowMouseCursor = false;
        bPauseMenuVisible = false;
    }
}

void AMainPlayerController::TogglePauseMenu()
{
    bPauseMenuVisible ? HidePauseMenu() : DisplayPauseMenu();
}

void AMainPlayerController::DisplayEnemyHealthBar()
{
    if (EnemyHealthBar)
    {
        bEnenmyHealthBarVisible = true;
        EnemyHealthBar->SetVisibility(ESlateVisibility::Visible);
    }
}

void AMainPlayerController::HideEnemyHealthBar()
{
    if (EnemyHealthBar)
    {
        bEnenmyHealthBarVisible = false;
        EnemyHealthBar->SetVisibility(ESlateVisibility::Hidden);
    }
}