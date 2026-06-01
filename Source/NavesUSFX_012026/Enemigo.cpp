#include "Enemigo.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/StaticMesh.h"

AEnemigo::AEnemigo()
{
    PrimaryActorTick.bCanEverTick = true;

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    RootComponent = MeshComponent;

    // Solo el enemigo base usa el OVNI
    static ConstructorHelpers::FObjectFinder<UStaticMesh> DefaultMesh(TEXT("/Game/TwinStick/Meshes/TwinStickUFO"));
    if (DefaultMesh.Succeeded())
    {
        MeshComponent->SetStaticMesh(DefaultMesh.Object);
    }

    MovementSpeed = 300.0f;
    AcceptanceRadius = 50.0f;
    CurrentWaypointIndex = 0;
    Lifespan = 0.0f;
}

void AEnemigo::BeginPlay()
{
    Super::BeginPlay();
    if (Lifespan > 0.0f)
    {
        SetLifeSpan(Lifespan);
    }

    if (Waypoints.Num() == 0)
    {
        FVector Base = GetActorLocation();
        Waypoints.Add(Base + FVector(0, 200, 0));
        Waypoints.Add(Base + FVector(200, 0, 0));
        Waypoints.Add(Base + FVector(0, -200, 0));
        Waypoints.Add(Base + FVector(-200, 0, 0));
    }

    if (Waypoints.Num() > 0)
    {
        CurrentWaypointIndex = 0;
    }
}

void AEnemigo::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    MoveToNextWaypoint(DeltaTime);
}

void AEnemigo::InitializeWaypoints(const TArray<FVector>& NewWaypoints)
{
    Waypoints = NewWaypoints;
    if (Waypoints.Num() > 0)
    {
        CurrentWaypointIndex = 0;
    }
}

void AEnemigo::MoveToNextWaypoint(float DeltaTime)
{
    if (Waypoints.Num() == 0) return;

    FVector CurrentLoc = GetActorLocation();
    FVector TargetLoc = Waypoints[CurrentWaypointIndex];
    FVector Dir = TargetLoc - CurrentLoc;
    float Distance = Dir.Size();

    if (Distance <= AcceptanceRadius)
    {
        CurrentWaypointIndex = (CurrentWaypointIndex + 1) % Waypoints.Num();
        TargetLoc = Waypoints[CurrentWaypointIndex];
        Dir = TargetLoc - CurrentLoc;
        Distance = Dir.Size();
    }

    if (Distance > 0)
    {
        FVector Movement = Dir.GetSafeNormal() * MovementSpeed * DeltaTime;
        if (Movement.Size() > Distance)
        {
            Movement = Dir;
        }
        SetActorLocation(CurrentLoc + Movement);
    }
}