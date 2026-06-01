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
		MeshComp->SetWorldScale3D(FVector(0.35f));
	}

	// Valores por defecto
	AutonomousSpeed = 250.0f;
	FormationSpeed = 800.0f;
	FormationAcceptanceRadius = 40.0f;

	bIsAutonomous = false;
	bMovingToFormation = false;
	bInFormation = false;

	TriangleAcceptanceRadius = 30.0f;
	CurrentVertexIndex = 0;

	FallLimit = -200.0f;
	TopZ = 600.0f;
}

void ANave::BeginPlay()
{
	Super::BeginPlay();
	InitialLocation = GetActorLocation();
	InitializeRandomMovement();
	SetMoveAutonomous(true);
}

void ANave::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsAutonomous)
	{
		switch (MovementType)
		{
		case EMovementType::Lluvia:    MoveLluvia(DeltaTime);    break;
		case EMovementType::Triangulo: MoveTriangulo(DeltaTime); break;
		case EMovementType::Circulo:   MoveCirculo(DeltaTime);   break;
		}
	}

	if (bMovingToFormation)
	{
		MoveToFormation(DeltaTime);
	}
}

// ---------- Control de estados ----------
void ANave::SetMoveAutonomous(bool bActive)
{
	bIsAutonomous = bActive;
	bMovingToFormation = false;
	bInFormation = false;
}

void ANave::StartFormation(const FVector& TargetLocation)
{
	bIsAutonomous = false;
	bMovingToFormation = true;
	FormationTarget = TargetLocation;
}

void ANave::ResumeAutonomous()
{
	bInFormation = false;
	SetMoveAutonomous(true);
}

// ---------- Inicialización aleatoria ----------
void ANave::InitializeRandomMovement()
{
	int32 RandomType = FMath::RandRange(0, 2); // 0 = Lluvia, 1 = Triángulo, 2 = Círculo
	MovementType = static_cast<EMovementType>(RandomType);

	AutonomousSpeed = FMath::RandRange(100.0f, 300.0f);

	switch (MovementType)
	{
	case EMovementType::Lluvia:
	{
		// Caída: dirección hacia abajo con pequeña variación lateral
		FallDirection = FVector(FMath::RandRange(-0.2f, 0.2f), FMath::RandRange(-0.2f, 0.2f), -1.0f).GetSafeNormal();
		// Altura aleatoria de reaparición
		TopZ = InitialLocation.Z + FMath::RandRange(100.0f, 300.0f);
		break;
	}
	case EMovementType::Triangulo:
	{
		// Definir vértices de un triángulo equilátero alrededor de la posición inicial
		float SideLength = FMath::RandRange(200.0f, 400.0f);
		float Height = SideLength * FMath::Sqrt(3.0f) / 2.0f;
		FVector Center = InitialLocation;
		// Vértices del triángulo (en XY, Z constante)
		TriangleVertices.Add(Center + FVector(0.0f, Height * 2.0f / 3.0f, 0.0f));     // Vértice superior
		TriangleVertices.Add(Center + FVector(-SideLength / 2.0f, -Height / 3.0f, 0.0f)); // Inferior izquierdo
		TriangleVertices.Add(Center + FVector(SideLength / 2.0f, -Height / 3.0f, 0.0f));  // Inferior derecho
		CurrentVertexIndex = 0;
		TriangleTarget = TriangleVertices[0];
		break;
	}
	case EMovementType::Circulo:
	{
		Radius = FMath::RandRange(150.0f, 350.0f);
		Angle = FMath::RandRange(0.0f, 360.0f);
		AngularSpeed = FMath::RandRange(30.0f, 90.0f); // grados por segundo
		break;
	}
	}
}

// ---------- Movimientos autónomos ----------
void ANave::MoveLluvia(float DeltaTime)
{
	FVector NewLocation = GetActorLocation() + FallDirection * AutonomousSpeed * DeltaTime;

	// Si ha bajado demasiado, reaparece arriba con una pequeña variación horizontal
	if (NewLocation.Z <= FallLimit)
	{
		NewLocation.Z = TopZ;
		NewLocation.X = InitialLocation.X + FMath::RandRange(-100.0f, 100.0f);
		NewLocation.Y = InitialLocation.Y + FMath::RandRange(-100.0f, 100.0f);
	}

	SetActorLocation(NewLocation);
}

void ANave::MoveTriangulo(float DeltaTime)
{
	FVector CurrentLoc = GetActorLocation();
	FVector Dir = TriangleTarget - CurrentLoc;
	float Dist = Dir.Size();

	if (Dist <= TriangleAcceptanceRadius)
	{
		// Cambiar al siguiente vértice
		CurrentVertexIndex = (CurrentVertexIndex + 1) % 3;
		TriangleTarget = TriangleVertices[CurrentVertexIndex];
		Dir = TriangleTarget - CurrentLoc;
		Dist = Dir.Size();
	}

	if (Dist > 0.0f)
	{
		FVector Movement = Dir.GetSafeNormal() * AutonomousSpeed * DeltaTime;
		if (Movement.Size() > Dist)
		{
			Movement = Dir;
		}
		SetActorLocation(CurrentLoc + Movement);
	}
}

void ANave::MoveCirculo(float DeltaTime)
{
	Angle += AngularSpeed * DeltaTime;
	if (Angle > 360.0f) Angle -= 360.0f;

	FVector NewLocation = InitialLocation;
	NewLocation.X += Radius * FMath::Cos(FMath::DegreesToRadians(Angle));
	NewLocation.Y += Radius * FMath::Sin(FMath::DegreesToRadians(Angle));
	NewLocation.Z = InitialLocation.Z;
	SetActorLocation(NewLocation);
}

// ---------- Movimiento a la formación ----------
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