#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enemigo.generated.h"

UCLASS()
class NAVESUSFX_012026_API AEnemigo : public AActor
{
    GENERATED_BODY()

public:
    AEnemigo();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;
    void InitializeWaypoints(const TArray<FVector>& NewWaypoints);

protected:
    UStaticMeshComponent* MeshComponent;
    float Lifespan;

private:
    TArray<FVector> Waypoints;
    int32 CurrentWaypointIndex;
    float MovementSpeed;
    float AcceptanceRadius;

    void MoveToNextWaypoint(float DeltaTime);
};