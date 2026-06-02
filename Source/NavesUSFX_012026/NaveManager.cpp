#include "NaveManager.h"
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
#include "Bloque.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"

void UNaveManager::Inicializar(UWorld* World)
{
    WorldRef = World;
    if (!WorldRef) return;

    // ---- Configuración de las cuadrillas ----
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

    // Inicio los labs
    SpawnSquad(Squad1Classes, Squad1Locations, Squad1Waypoints, Squad1Members);
    WorldRef->GetTimerManager().SetTimer(CheckSquad1Timer, this,
        &UNaveManager::CheckSquad1Status, 1.0f, true);

    SpawnNaves();
    WorldRef->GetTimerManager().SetTimer(TimerFormacion, this,
        &UNaveManager::FormNavesFrenteAlPawn, 5.0f, false);

    GenerarLaberintoConBloques();
}

// Lab 2
void UNaveManager::SpawnSquad(const TArray<TSubclassOf<AEnemigo>>& Classes,
    const TArray<FVector>& Locations,
    const TArray<FVector>& Waypoints,
    TArray<TWeakObjectPtr<AEnemigo>>& OutMembers)
{
    if (!WorldRef) return;
    OutMembers.Empty();
    for (int32 i = 0; i < Classes.Num(); ++i)
    {
        if (!Classes[i]) continue;
        FVector Loc = Locations.IsValidIndex(i) ? Locations[i] : FVector::ZeroVector;
        FRotator Rot = FRotator::ZeroRotator;
        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride =
            ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

        AEnemigo* NewEnemy = WorldRef->SpawnActor<AEnemigo>(Classes[i], Loc, Rot, SpawnParams);
        if (NewEnemy)
        {
            NewEnemy->InitializeWaypoints(Waypoints);
            OutMembers.Add(NewEnemy);
        }
    }
}

void UNaveManager::CheckSquad1Status()
{
    bool bAllGone = true;
    for (const TWeakObjectPtr<AEnemigo>& Ptr : Squad1Members)
        if (Ptr.IsValid()) { bAllGone = false; break; }

    if (bAllGone)
    {
        WorldRef->GetTimerManager().ClearTimer(CheckSquad1Timer);
        SpawnSquad2();
    }
}

void UNaveManager::SpawnSquad2()
{
    TArray<TWeakObjectPtr<AEnemigo>> Squad2Members;
    SpawnSquad(Squad2Classes, Squad2Locations, Squad2Waypoints, Squad2Members);
}

//  Lab 3 
void UNaveManager::SpawnNaves()
{
    if (!WorldRef) return;
    NavesContenedor.Empty();
    for (int32 i = 0; i < 20; ++i)
    {
        float X = FMath::RandRange(-800.0f, 800.0f);
        float Y = FMath::RandRange(-800.0f, 800.0f);
        FVector SpawnLoc(X, Y, 220.0f);
        FRotator SpawnRot = FRotator::ZeroRotator;
        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride =
            ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

        ANave* NewNave = WorldRef->SpawnActor<ANave>(ANave::StaticClass(), SpawnLoc, SpawnRot, SpawnParams);
        if (NewNave) NavesContenedor.Add(NewNave);
    }
}

void UNaveManager::FormNavesFrenteAlPawn()
{
    if (!WorldRef) return;
    APlayerController* PC = WorldRef->GetFirstPlayerController();
    if (!PC) return;
    APawn* Pawn = PC->GetPawn();
    if (!Pawn) return;

    FVector PawnLocation = Pawn->GetActorLocation();
    FVector PawnForward = Pawn->GetActorForwardVector();
    FVector PawnRight = Pawn->GetActorRightVector();
    FVector BaseTarget = PawnLocation + PawnForward * 500.0f;

    int32 Columnas = 5, Filas = 4;
    float EspaciadoH = 150.0f, EspaciadoV = 150.0f;
    int32 index = 0;
    for (int32 fila = 0; fila < Filas; ++fila)
    {
        for (int32 col = 0; col < Columnas; ++col)
        {
            if (index >= NavesContenedor.Num()) break;
            float OffsetX = (col - Columnas / 2) * EspaciadoH;
            float OffsetY = (fila - Filas / 2) * EspaciadoV;
            FVector Target = BaseTarget + PawnRight * OffsetX + FVector(0, 0, OffsetY);
            Target.Z = PawnLocation.Z;
            NavesContenedor[index]->StartFormation(Target);
            ++index;
        }
    }
    WorldRef->GetTimerManager().SetTimer(TimerRetorno, this,
        &UNaveManager::ReturnNavesAutonomous, 5.0f, false);
}

void UNaveManager::ReturnNavesAutonomous()
{
    for (ANave* Nave : NavesContenedor)
        if (Nave) Nave->ResumeAutonomous();
}

//  Lab 4 
void UNaveManager::GenerarLaberintoConBloques()
{
    if (!WorldRef) return;
    const int32 Filas = 17, Columnas = 16;
    int32 Laberinto[Filas][Columnas] =
    {
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,1},
        {0,0,1,0,1,0,1,1,1,1,0,1,0,1,0,1},
        {0,0,1,0,0,0,0,0,0,1,0,0,0,1,0,1},
        {1,0,1,1,1,1,1,1,0,1,1,1,1,1,0,1},
        {1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1},
        {1,0,1,1,1,1,0,1,0,1,1,1,1,1,0,1},
        {1,0,1,0,0,0,0,1,0,1,0,0,0,0,0,1},
        {1,0,1,0,1,1,1,1,0,1,0,1,1,1,0,1},
        {1,0,0,0,0,0,0,0,0,1,0,0,0,1,0,1},
        {1,1,1,1,1,1,1,1,0,1,1,1,0,1,0,1},
        {1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1},
        {1,0,1,1,1,1,1,1,0,1,0,1,1,1,0,1},
        {1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1},
        {1,1,1,1,1,1,0,1,1,1,1,1,1,1,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
    };

    const float TamanoCelda = 150.0f;
    const float OffsetX = -1200.0f, OffsetY = -1275.0f, AlturaZ = 220.0f;

    for (int32 fila = 0; fila < Filas; ++fila)
    {
        for (int32 col = 0; col < Columnas; ++col)
        {
            if (Laberinto[fila][col] == 1)
            {
                float X = OffsetX + col * TamanoCelda + TamanoCelda * 0.5f;
                float Y = OffsetY + fila * TamanoCelda + TamanoCelda * 0.5f;
                FVector Pos(X, Y, AlturaZ);
                FRotator Rot = FRotator::ZeroRotator;
                FActorSpawnParameters SpawnParams;
                SpawnParams.SpawnCollisionHandlingOverride =
                    ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

                ABloque* Bloque = WorldRef->SpawnActor<ABloque>(ABloque::StaticClass(), Pos, Rot, SpawnParams);
                if (Bloque)
                {
                    Bloque->MeshComp->SetWorldScale3D(FVector(1.5f, 1.5f, 1.0f));
                    ListaBloquesLaberinto.insertar(Bloque);
                }
            }
        }
    }

    auto CentroCelda = [&](int32 Col, int32 Fila) -> FVector
    {
        return FVector(OffsetX + Col * TamanoCelda + TamanoCelda * 0.5f,
            OffsetY + Fila * TamanoCelda + TamanoCelda * 0.5f, AlturaZ);
    };

    TArray<FVector> RutaLaberinto;
    FVector EntradaExterna = CentroCelda(1, 1) - FVector(100.0f, 0.0f, 0.0f);
    RutaLaberinto.Add(EntradaExterna);
    RutaLaberinto.Add(CentroCelda(1, 1));
    RutaLaberinto.Add(CentroCelda(2, 1));
    RutaLaberinto.Add(CentroCelda(3, 1));
    RutaLaberinto.Add(CentroCelda(3, 2));
    RutaLaberinto.Add(CentroCelda(3, 3));
    RutaLaberinto.Add(CentroCelda(3, 4));
    RutaLaberinto.Add(CentroCelda(3, 5));
    RutaLaberinto.Add(CentroCelda(4, 5));
    RutaLaberinto.Add(CentroCelda(5, 5));
    RutaLaberinto.Add(CentroCelda(6, 5));
    RutaLaberinto.Add(CentroCelda(7, 5));
    RutaLaberinto.Add(CentroCelda(8, 5));
    RutaLaberinto.Add(CentroCelda(9, 5));
    RutaLaberinto.Add(CentroCelda(10, 5));
    RutaLaberinto.Add(CentroCelda(11, 5));
    RutaLaberinto.Add(CentroCelda(11, 6));
    RutaLaberinto.Add(CentroCelda(11, 7));
    RutaLaberinto.Add(CentroCelda(11, 8));
    RutaLaberinto.Add(CentroCelda(11, 9));
    RutaLaberinto.Add(CentroCelda(11, 10));
    RutaLaberinto.Add(CentroCelda(11, 11));
    RutaLaberinto.Add(CentroCelda(12, 11));
    RutaLaberinto.Add(CentroCelda(13, 11));
    RutaLaberinto.Add(CentroCelda(14, 11));
    RutaLaberinto.Add(CentroCelda(14, 12));
    RutaLaberinto.Add(CentroCelda(14, 13));
    RutaLaberinto.Add(CentroCelda(14, 14));
    RutaLaberinto.Add(CentroCelda(15, 15));
    FVector SalidaExterna = CentroCelda(15, 15) + FVector(100.0f, 0.0f, 0.0f);
    RutaLaberinto.Add(SalidaExterna);

    TArray<TSubclassOf<AEnemigo>> PatrullaClases;
    PatrullaClases.Add(AEADron::StaticClass());
    PatrullaClases.Add(AEADron::StaticClass());
    PatrullaClases.Add(AEADron::StaticClass());

    TArray<FVector> PatrullaSpawns;
    PatrullaSpawns.Add(EntradaExterna + FVector(0, 20, 0));
    PatrullaSpawns.Add(EntradaExterna - FVector(0, 20, 0));
    PatrullaSpawns.Add(EntradaExterna + FVector(20, 0, 0));

    TArray<TWeakObjectPtr<AEnemigo>> PatrullaMembers;
    SpawnSquad(PatrullaClases, PatrullaSpawns, RutaLaberinto, PatrullaMembers);
}