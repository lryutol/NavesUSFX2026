#include "Enemigo.h"
#include "Bloque.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/StaticMesh.h"

AEnemigo::AEnemigo()
{
    PrimaryActorTick.bCanEverTick = true;

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    RootComponent = MeshComponent;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> DefaultMesh(TEXT("/Game/TwinStick/Meshes/TwinStickUFO"));
    if (DefaultMesh.Succeeded())
    {
        MeshComponent->SetStaticMesh(DefaultMesh.Object);
    }

    // Configuración de colisiones
    MeshComponent->SetNotifyRigidBodyCollision(true);
    MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    MeshComponent->SetCollisionObjectType(ECC_WorldDynamic);
    MeshComponent->SetCollisionResponseToAllChannels(ECR_Block);

    MovementSpeed = 300.0f;
    AcceptanceRadius = 50.0f;
    CurrentWaypointIndex = 0;
    Lifespan = 0.0f;

    bIsBouncing = false;
    BounceTimer = 0.0f;
}

void AEnemigo::BeginPlay()
{
    Super::BeginPlay();

    // Vincular el evento de colisión
    MeshComponent->OnComponentHit.AddDynamic(this, &AEnemigo::OnHit);

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

    // Si está rebotando, disminuir el temporizador
    if (bIsBouncing)
    {
        BounceTimer -= DeltaTime;
        if (BounceTimer <= 0.0f)
        {
            bIsBouncing = false;
            BounceDirection = FVector::ZeroVector;
        }
    }

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
    FVector Dir;

    if (bIsBouncing && !BounceDirection.IsNearlyZero())
    {
        // Durante el rebote, moverse en la dirección de rebote
        Dir = BounceDirection;
    }
    else
    {
        // Movimiento normal hacia el waypoint
        Dir = TargetLoc - CurrentLoc;
        float Dist = Dir.Size();
        if (Dist <= AcceptanceRadius)
        {
            CurrentWaypointIndex = (CurrentWaypointIndex + 1) % Waypoints.Num();
            TargetLoc = Waypoints[CurrentWaypointIndex];
            Dir = TargetLoc - CurrentLoc;
            Dist = Dir.Size();
        }
    }

    if (Dir.IsNearlyZero()) return;

    // Calcular movimiento
    FVector Movement = Dir.GetSafeNormal() * MovementSpeed * DeltaTime;
    float DistToTarget = Dir.Size();
    if (Movement.Size() > DistToTarget && !bIsBouncing)
    {
        Movement = Dir; // Llegar justo al waypoint
    }

    // Intentar mover con barrido
    FHitResult Hit;
    SetActorLocation(CurrentLoc + Movement, true, &Hit);

    // Si chocó con algo, activar rebote
    if (Hit.bBlockingHit)
    {
        // Solo si no está ya rebotando, iniciar rebote
        if (!bIsBouncing)
        {
            bIsBouncing = true;
            BounceTimer = 0.5f;
            FVector HitNormal = Hit.Normal;
            // Dirección de rebote: reflejar la dirección de movimiento respecto a la normal
            FVector MoveDir = Movement.GetSafeNormal();
            BounceDirection = FMath::GetReflectionVector(MoveDir, HitNormal).GetSafeNormal();
        }
    }
}

void AEnemigo::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    // Este evento se usa como respaldo; la lógica principal está en MoveToNextWaypoint
    if (OtherActor && OtherActor->IsA(ABloque::StaticClass()))
    {
        if (!bIsBouncing)
        {
            bIsBouncing = true;
            BounceTimer = 0.5f;
            FVector CurrentDir = (GetActorLocation() - Hit.Location).GetSafeNormal();
            BounceDirection = FMath::GetReflectionVector(CurrentDir, Hit.Normal).GetSafeNormal();
        }
    }
}