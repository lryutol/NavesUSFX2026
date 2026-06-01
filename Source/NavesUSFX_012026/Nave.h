#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Nave.generated.h"

UCLASS()
class NAVESUSFX_012026_API ANave : public AActor
{
    GENERATED_BODY()

public:
    ANave();
    virtual void Tick(float DeltaTime) override;

    void StartFormation(const FVector& TargetLocation);
    void ResumeAutonomous();

protected:
    virtual void BeginPlay() override;
    UStaticMeshComponent* MeshComp;

private:
    enum class EMovementType : uint8
    {
        Circular,
        Cuadrado,
        Zigzag,
        Patrullaje
    };
    EMovementType MovementType;

    bool bAutonomousReady;
    bool bMovingToFormation;
    bool bInFormation;

    FVector InitialLocation;
    float AutonomousSpeed;
    float Radius;
    float Angle;
    float TimeAccum;
    FVector MoveDirection;
    float ZigzagTimer;
    bool bZigzagRight;
    TArray<FVector> CuadradoVertices;
    int32 CurrentVertex;
    FVector TargetVertex;

    FVector FormationTarget;
    float FormationSpeed;
    float FormationAcceptanceRadius;

    void InitializeRandomMovement();
    void MoveCircular(float DeltaTime);
    void MoveCuadrado(float DeltaTime);
    void MoveZigzag(float DeltaTime);
    void MovePatrullaje(float DeltaTime);
    void MoveToFormation(float DeltaTime);
};