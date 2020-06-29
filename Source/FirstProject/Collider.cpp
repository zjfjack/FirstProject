// Fill out your copyright notice in the Description page of Project Settings.


#include "Collider.h"
#include "Components/SphereComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "ColliderMovementComponent.h"

// Sets default values
ACollider::ACollider() :
    MeshComponent(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"))),
    SphereComponent(CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"))),
    CameraComponent(CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"))),
    SpringArmComponent(CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"))),
    ColliderMovementComponent(CreateDefaultSubobject<UColliderMovementComponent>(TEXT("ColliderMovementComponent"))),
    CameraInput(FVector2D(0.f))
{
    // Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    SetRootComponent(SphereComponent);
    SphereComponent->InitSphereRadius(40.f);
    SphereComponent->SetCollisionProfileName(TEXT("Pawn"));

    MeshComponent->SetupAttachment(GetRootComponent());
    ConstructorHelpers::FObjectFinder<UStaticMesh> MeshComponentAsset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));
    if (MeshComponentAsset.Succeeded())
    {
        MeshComponent->SetStaticMesh(MeshComponentAsset.Object);
        MeshComponent->SetRelativeLocation(FVector(0.f, 0.f, -40.f));
        MeshComponent->SetWorldScale3D(FVector(0.8f));
    }

    SpringArmComponent->SetupAttachment(GetRootComponent());
    SpringArmComponent->SetRelativeRotation(FRotator(-45.f, 0.f, 0.f));
    SpringArmComponent->TargetArmLength = 400.f;
    SpringArmComponent->bEnableCameraLag = true;
    SpringArmComponent->CameraLagSpeed = 3.f;

    ColliderMovementComponent->UpdatedComponent = RootComponent;

    CameraComponent->SetupAttachment(GetSpringArmComponent(), USpringArmComponent::SocketName);
}

// Called when the game starts or when spawned
void ACollider::BeginPlay()
{
    Super::BeginPlay();
    
}

// Called every frame
void ACollider::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    FRotator NewActorRotation = GetActorRotation();
    NewActorRotation.Yaw += CameraInput.X;
    SetActorRotation(NewActorRotation);

    FRotator NewSpringArmRotation = SpringArmComponent->GetComponentRotation();
    NewSpringArmRotation.Pitch = FMath::Clamp(NewSpringArmRotation.Pitch + CameraInput.Y, -80.f, -15.f);
    SpringArmComponent->SetWorldRotation(NewSpringArmRotation);
}

// Called to bind functionality to input
void ACollider::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ACollider::MoveForward);
    PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ACollider::MoveRight);
    PlayerInputComponent->BindAxis(TEXT("CameraPitch"), this, &ACollider::PitchCamera);
    PlayerInputComponent->BindAxis(TEXT("CameraYaw"), this, &ACollider::YawCamera);
}

UPawnMovementComponent* ACollider::GetMovementComponent() const
{
    return ColliderMovementComponent;
}

void ACollider::MoveForward(float Value)
{
    FVector Foward = GetActorForwardVector();
    if (ColliderMovementComponent)
        ColliderMovementComponent->AddInputVector(Value * Foward);
}

void ACollider::MoveRight(float Value)
{
    FVector Right = GetActorRightVector();
    if (ColliderMovementComponent)
        ColliderMovementComponent->AddInputVector(Value * Right);
}

void ACollider::PitchCamera(float Value)
{
    CameraInput.Y = Value;
}

void ACollider::YawCamera(float Value)
{
    CameraInput.X = Value;
}