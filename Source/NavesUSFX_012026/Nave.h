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

	void SetMoveAutonomous(bool bActive);
	void StartFormation(const FVector& TargetLocation);
	void ResumeAutonomous();

protected:
	virtual void BeginPlay() override;

	UStaticMeshComponent* MeshComp;

private:
	enum class EMovementType : uint8
	{
		Lluvia,    // Caída vertical, como gotas de lluvia
		Triangulo, // Recorrido en forma de triángulo equilátero
		Circulo    // Movimiento circular alrededor de un punto central
	};
	EMovementType MovementType;

	bool bIsAutonomous;
	bool bMovingToFormation;
	bool bInFormation;

	// Variables comunes
	FVector InitialLocation;
	float AutonomousSpeed;

	// Lluvia
	FVector FallDirection;     // Dirección de caída (normalmente hacia abajo)
	float FallLimit;           // Coordenada Z mínima (para reiniciar arriba)
	float TopZ;                // Altura de reaparición

	// Triángulo
	TArray<FVector> TriangleVertices;
	int32 CurrentVertexIndex;
	FVector TriangleTarget;
	float TriangleAcceptanceRadius;

	// Círculo
	float Radius;
	float Angle;
	float AngularSpeed;

	// Formación
	FVector FormationTarget;
	float FormationSpeed;
	float FormationAcceptanceRadius;

	// Inicialización aleatoria
	void InitializeRandomMovement();

	// Movimientos
	void MoveLluvia(float DeltaTime);
	void MoveTriangulo(float DeltaTime);
	void MoveCirculo(float DeltaTime);
	void MoveToFormation(float DeltaTime);
};