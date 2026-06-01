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

    // Permite al GameMode asignar puntos de patrulla después del spawn
    void InitializeWaypoints(const TArray<FVector>& NewWaypoints);

private:
    UStaticMeshComponent* MeshComponent;    // Componente de malla
    TArray<FVector> Waypoints;              // Lista de puntos a recorrer
    int32 CurrentWaypointIndex;             // Índice actual del waypoint
    float MovementSpeed;                    // Velocidad en unidades/segundo
    float AcceptanceRadius;                 // Distancia para cambiar de punto

    void MoveToNextWaypoint(float DeltaTime);
};