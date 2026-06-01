#include "Nave.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/StaticMesh.h"

ANave::ANave()
{
    PrimaryActorTick.bCanEverTick = true;

    MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
    RootComponent = MeshComp;
    static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh(TEXT("/Engine/BasicShapes/Sphere"));
    if (SphereMesh.Succeeded())
    {
        MeshComp->SetStaticMesh(SphereMesh.Object);
        MeshComp->SetWorldScale3D(FVector(0.4f));
    }

    AutonomousSpeed = 250.0f;
    FormationSpeed = 800.0f;
    FormationAcceptanceRadius = 40.0f;
    bAutonomousReady = false;
    bMovingToFormation = false;
    bInFormation = false;
    TimeAccum = 0.0f;
    ZigzagTimer = 0.0f;
    bZigzagRight = true;
    CurrentVertex = 0;
}

void ANave::BeginPlay()
{
    Super::BeginPlay();
    InitialLocation = GetActorLocation();
    InitializeRandomMovement();
    bAutonomousReady = true;
}

void ANave::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bAutonomousReady)
    {
        switch (MovementType)
        {
        case EMovementType::Circular:    MoveCircular(DeltaTime);   break;
        case EMovementType::Cuadrado:    MoveCuadrado(DeltaTime);   break;
        case EMovementType::Zigzag:      MoveZigzag(DeltaTime);     break;
        case EMovementType::Patrullaje:  MovePatrullaje(DeltaTime); break;
        }
    }

    if (bMovingToFormation)
    {
        MoveToFormation(DeltaTime);
    }
}

void ANave::StartFormation(const FVector& TargetLocation)
{
    bAutonomousReady = false;
    bMovingToFormation = true;
    FormationTarget = TargetLocation;
}

void ANave::ResumeAutonomous()
{
    bInFormation = false;
    bMovingToFormation = false;
    bAutonomousReady = true;
}

void ANave::InitializeRandomMovement()
{
    int32 RandomType = FMath::RandRange(0, 3);
    MovementType = static_cast<EMovementType>(RandomType);
    AutonomousSpeed = FMath::RandRange(150.0f, 350.0f);

    switch (MovementType)
    {
    case EMovementType::Circular:
        Radius = FMath::RandRange(100.0f, 300.0f);
        Angle = FMath::RandRange(0.0f, 360.0f);
        break;
    case EMovementType::Cuadrado:
    {
        float SideLength = FMath::RandRange(150.0f, 300.0f);
        FVector Center = InitialLocation;
        CuadradoVertices.Add(Center + FVector(-SideLength / 2, -SideLength / 2, 0));
        CuadradoVertices.Add(Center + FVector(SideLength / 2, -SideLength / 2, 0));
        CuadradoVertices.Add(Center + FVector(SideLength / 2, SideLength / 2, 0));
        CuadradoVertices.Add(Center + FVector(-SideLength / 2, SideLength / 2, 0));
        CurrentVertex = 0;
        TargetVertex = CuadradoVertices[0];
        break;
    }
    case EMovementType::Zigzag:
        MoveDirection = FVector(FMath::RandRange(-1.0f, 1.0f), FMath::RandRange(-1.0f, 1.0f), 0.0f).GetSafeNormal();
        ZigzagTimer = 0.0f;
        bZigzagRight = true;
        break;
    case EMovementType::Patrullaje:
        MoveDirection = FVector(FMath::RandRange(-1.0f, 1.0f), FMath::RandRange(-1.0f, 1.0f), 0.0f).GetSafeNormal();
        TimeAccum = 0.0f;
        break;
    }
}

void ANave::MoveCircular(float DeltaTime)
{
    Angle += AutonomousSpeed * DeltaTime * 0.5f;
    if (Angle > 360.0f) Angle -= 360.0f;
    FVector NewLocation = InitialLocation;
    NewLocation.X += Radius * FMath::Cos(FMath::DegreesToRadians(Angle));
    NewLocation.Y += Radius * FMath::Sin(FMath::DegreesToRadians(Angle));
    NewLocation.Z = InitialLocation.Z;
    SetActorLocation(NewLocation);
}

void ANave::MoveCuadrado(float DeltaTime)
{
    FVector CurrentLoc = GetActorLocation();
    FVector Dir = TargetVertex - CurrentLoc;
    float Dist = Dir.Size();
    if (Dist <= 10.0f)
    {
        CurrentVertex = (CurrentVertex + 1) % CuadradoVertices.Num();
        TargetVertex = CuadradoVertices[CurrentVertex];
        Dir = TargetVertex - CurrentLoc;
        Dist = Dir.Size();
    }
    if (Dist > 0.0f)
    {
        FVector Movement = Dir.GetSafeNormal() * AutonomousSpeed * DeltaTime;
        if (Movement.Size() > Dist) Movement = Dir;
        SetActorLocation(CurrentLoc + Movement);
    }
}

void ANave::MoveZigzag(float DeltaTime)
{
    FVector Location = GetActorLocation();
    Location += MoveDirection * AutonomousSpeed * DeltaTime;

    ZigzagTimer += DeltaTime;
    if (ZigzagTimer > 1.5f)
    {
        bZigzagRight = !bZigzagRight;
        ZigzagTimer = 0.0f;
    }
    FVector Perp = FVector(MoveDirection.Y, -MoveDirection.X, 0.0f) * (bZigzagRight ? 1.0f : -1.0f);
    Location += Perp * 200.0f * DeltaTime;

    if (FMath::Abs(Location.X) > 1200.0f || FMath::Abs(Location.Y) > 1200.0f)
    {
        MoveDirection = MoveDirection.RotateAngleAxis(45.0f, FVector::UpVector);
        Location = GetActorLocation();
    }
    Location.Z = InitialLocation.Z;
    SetActorLocation(Location);
}

void ANave::MovePatrullaje(float DeltaTime)
{
    TimeAccum += DeltaTime;
    FVector Location = GetActorLocation();
    Location += MoveDirection * AutonomousSpeed * DeltaTime;

    if (TimeAccum > 3.0f)
    {
        MoveDirection = FVector(FMath::RandRange(-1.0f, 1.0f), FMath::RandRange(-1.0f, 1.0f), 0.0f).GetSafeNormal();
        TimeAccum = 0.0f;
    }

    if (FMath::Abs(Location.X) > 1200.0f || FMath::Abs(Location.Y) > 1200.0f)
    {
        MoveDirection = -MoveDirection;
        Location = GetActorLocation();
    }
    Location.Z = InitialLocation.Z;
    SetActorLocation(Location);
}

void ANave::MoveToFormation(float DeltaTime)
{
    FVector CurrentLoc = GetActorLocation();
    FVector NewLoc = FMath::VInterpConstantTo(CurrentLoc, FormationTarget, DeltaTime, FormationSpeed);
    if (FVector::Dist(NewLoc, FormationTarget) <= FormationAcceptanceRadius)
    {
        NewLoc = FormationTarget;
        bMovingToFormation = false;
        bInFormation = true;
    }
    SetActorLocation(NewLoc);
}