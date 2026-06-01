#include "NavesUSFX_012026Gamemode.h"
#include "Nave.h"
#include "Enemigo.h"
#include "EAHelicoptero.h"
#include "EAAvion.h"
#include "EADron.h"
#include "ETSoldado.h"
#include "ETTanque.h"
#include "ETCamion.h"
#include "ETBlindado.h"
#include "EABarco.h"
#include "EALancha.h"
#include "EAMotoAcuatica.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"
#include "Kismet/GameplayStatics.h"

ANavesUSFX_012026Gamemode::ANavesUSFX_012026Gamemode()
{
	// ===== Cuadrilla 1 =====
	Squad1Classes.Add(AEAHelicoptero::StaticClass());
	Squad1Classes.Add(AEAAvion::StaticClass());
	Squad1Classes.Add(AETSoldado::StaticClass());
	Squad1Classes.Add(AETTanque::StaticClass());

	Squad1Locations.Add(FVector(200, 0, 220));
	Squad1Locations.Add(FVector(0, 200, 220));
	Squad1Locations.Add(FVector(-200, 0, 220));
	Squad1Locations.Add(FVector(0, -200, 220));

	Squad1Waypoints.Add(FVector(500, 0, 220));
	Squad1Waypoints.Add(FVector(0, 500, 220));
	Squad1Waypoints.Add(FVector(-500, 0, 220));
	Squad1Waypoints.Add(FVector(0, -500, 220));

	// ===== Cuadrilla 2 =====
	Squad2Classes.Add(AEABarco::StaticClass());
	Squad2Classes.Add(AEALancha::StaticClass());
	Squad2Classes.Add(AEAMotoAcuatica::StaticClass());
	Squad2Classes.Add(AETCamion::StaticClass());

	Squad2Locations.Add(FVector(300, 300, 220));
	Squad2Locations.Add(FVector(-300, 300, 220));
	Squad2Locations.Add(FVector(-300, -300, 220));
	Squad2Locations.Add(FVector(300, -300, 220));

	Squad2Waypoints.Add(FVector(600, 600, 220));
	Squad2Waypoints.Add(FVector(-600, 600, 220));
	Squad2Waypoints.Add(FVector(-600, -600, 220));
	Squad2Waypoints.Add(FVector(600, -600, 220));
}

void ANavesUSFX_012026Gamemode::BeginPlay()
{
	Super::BeginPlay();

	// 1. Spawnear los enemigos de las cuadrillas (Lab2)
	SpawnSquad(Squad1Classes, Squad1Locations, Squad1Waypoints, Squad1Members);
	GetWorldTimerManager().SetTimer(CheckSquad1Timer, this, &ANavesUSFX_012026Gamemode::CheckSquad1Status, 1.0f, true);

	// 2. Spawnear las 20 naves del Lab3
	SpawnNaves();

	// Programar la formación después de 5 segundos
	GetWorldTimerManager().SetTimer(TimerFormacion, this, &ANavesUSFX_012026Gamemode::FormNavesFrenteAlPawn, 5.0f, false);
}

// ---------- Métodos de las cuadrillas (sin cambios) ----------
void ANavesUSFX_012026Gamemode::SpawnSquad(const TArray<TSubclassOf<AEnemigo>>& Classes, const TArray<FVector>& Locations, const TArray<FVector>& Waypoints, TArray<TWeakObjectPtr<AEnemigo>>& OutMembers)
{
	UWorld* World = GetWorld();
	if (!World) return;
	OutMembers.Empty();
	for (int32 i = 0; i < Classes.Num(); ++i)
	{
		if (!Classes[i]) continue;
		FVector Loc = Locations.IsValidIndex(i) ? Locations[i] : FVector::ZeroVector;
		FRotator Rot = FRotator::ZeroRotator;
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		AEnemigo* NewEnemy = World->SpawnActor<AEnemigo>(Classes[i], Loc, Rot, SpawnParams);
		if (NewEnemy)
		{
			NewEnemy->InitializeWaypoints(Waypoints);
			OutMembers.Add(NewEnemy);
		}
	}
}

void ANavesUSFX_012026Gamemode::CheckSquad1Status()
{
	bool bAllGone = true;
	for (const TWeakObjectPtr<AEnemigo>& Ptr : Squad1Members)
		if (Ptr.IsValid()) { bAllGone = false; break; }
	if (bAllGone)
	{
		GetWorldTimerManager().ClearTimer(CheckSquad1Timer);
		SpawnSquad2();
	}
}

void ANavesUSFX_012026Gamemode::SpawnSquad2()
{
	TArray<TWeakObjectPtr<AEnemigo>> Squad2Members;
	SpawnSquad(Squad2Classes, Squad2Locations, Squad2Waypoints, Squad2Members);
}

// ---------- Nuevos métodos para las 20 naves ----------
void ANavesUSFX_012026Gamemode::SpawnNaves()
{
	UWorld* World = GetWorld();
	if (!World) return;

	NavesContenedor.Empty();

	for (int32 i = 0; i < 20; ++i)
	{
		// Posición aleatoria dentro del área de juego
		float X = FMath::RandRange(-800.0f, 800.0f);
		float Y = FMath::RandRange(-800.0f, 800.0f);
		float Z = 220.0f; // misma altura que el pawn
		FVector SpawnLoc(X, Y, Z);
		FRotator SpawnRot = FRotator::ZeroRotator;
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		ANave* NuevaNave = World->SpawnActor<ANave>(ANave::StaticClass(), SpawnLoc, SpawnRot, SpawnParams);
		if (NuevaNave)
		{
			NavesContenedor.Add(NuevaNave);
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("20 naves spawneadas."));
}

void ANavesUSFX_012026Gamemode::FormNavesFrenteAlPawn()
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC) return;
	APawn* Pawn = PC->GetPawn();
	if (!Pawn) return;

	FVector PawnLocation = Pawn->GetActorLocation();
	FVector PawnForward = Pawn->GetActorForwardVector();
	FVector PawnRight = Pawn->GetActorRightVector();

	// Punto central frente al pawn (500 unidades adelante)
	FVector BaseTarget = PawnLocation + PawnForward * 500.0f;

	// Configuración de la formación (5 columnas x 4 filas = 20 naves)
	int32 Columnas = 5;
	int32 Filas = 4;
	float EspaciadoHorizontal = 150.0f;
	float EspaciadoVertical = 150.0f;

	int32 index = 0;
	for (int32 fila = 0; fila < Filas; ++fila)
	{
		for (int32 col = 0; col < Columnas; ++col)
		{
			if (index >= NavesContenedor.Num()) break;

			// Desplazamiento respecto al centro
			float OffsetX = (col - Columnas / 2) * EspaciadoHorizontal; // centrar
			float OffsetY = (fila - Filas / 2) * EspaciadoVertical;
			FVector Target = BaseTarget + PawnRight * OffsetX + FVector(0.0f, 0.0f, OffsetY); // Up sería Z
			// Queremos que la formación quede en el mismo plano Z que el pawn, así que mantenemos Z = PawnLocation.Z
			Target.Z = PawnLocation.Z;

			NavesContenedor[index]->StartFormation(Target);
			++index;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Naves en formación frente al Pawn."));

	// Programar retorno al movimiento autónomo después de 5 segundos
	GetWorldTimerManager().SetTimer(TimerRetorno, this, &ANavesUSFX_012026Gamemode::ReturnNavesAutonomous, 5.0f, false);
}

void ANavesUSFX_012026Gamemode::ReturnNavesAutonomous()
{
	for (ANave* Nave : NavesContenedor)
	{
		if (Nave)
		{
			Nave->ResumeAutonomous();
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Naves retornan a movimiento autónomo."));
}