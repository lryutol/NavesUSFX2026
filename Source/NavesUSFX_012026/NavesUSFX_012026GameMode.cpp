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
#include "Bloque.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"

ANavesUSFX_012026Gamemode::ANavesUSFX_012026Gamemode()
{
    // ===== Lab 2 =====
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
}

void ANavesUSFX_012026Gamemode::BeginPlay()
{
    Super::BeginPlay();

    // Lab 2
    SpawnSquad(Squad1Classes, Squad1Locations, Squad1Waypoints, Squad1Members);
    GetWorldTimerManager().SetTimer(CheckSquad1Timer, this,
        &ANavesUSFX_012026Gamemode::CheckSquad1Status,
        1.0f, true);

    // Lab 3
    SpawnNaves();
    GetWorldTimerManager().SetTimer(TimerFormacion, this,
        &ANavesUSFX_012026Gamemode::FormNavesFrenteAlPawn,
        5.0f, false);

    // Lab 4
    GenerarLaberintoConBloques();
}

// ========== Métodos Lab 2 (sin cambios) ==========
void ANavesUSFX_012026Gamemode::SpawnSquad(const TArray<TSubclassOf<AEnemigo>>& Classes,
    const TArray<FVector>& Locations,
    const TArray<FVector>& Waypoints,
    TArray<TWeakObjectPtr<AEnemigo>>& OutMembers)
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
        SpawnParams.SpawnCollisionHandlingOverride =
            ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

        AEnemigo* NewEnemy = World->SpawnActor<AEnemigo>(Classes[i], Loc, Rot, SpawnParams);
        if (NewEnemy)
        {
            NewEnemy->InitializeWaypoints(Waypoints);
            OutMembers.Add(NewEnemy);
            UE_LOG(LogTemp, Log, TEXT("Enemigo de cuadrilla spawneado en %s"), *Loc.ToString());
        }
    }
}

void ANavesUSFX_012026Gamemode::CheckSquad1Status()
{
    bool bAllGone = true;
    for (const TWeakObjectPtr<AEnemigo>& Ptr : Squad1Members)
    {
        if (Ptr.IsValid())
        {
            bAllGone = false;
            break;
        }
    }

    if (bAllGone)
    {
        GetWorldTimerManager().ClearTimer(CheckSquad1Timer);
        UE_LOG(LogTemp, Warning, TEXT("Primera cuadrilla eliminada. Aparece segunda cuadrilla."));
        SpawnSquad2();
    }
}

void ANavesUSFX_012026Gamemode::SpawnSquad2()
{
    TArray<TWeakObjectPtr<AEnemigo>> Squad2Members;
    SpawnSquad(Squad2Classes, Squad2Locations, Squad2Waypoints, Squad2Members);
}

// ========== Métodos Lab 3 (sin cambios) ==========
void ANavesUSFX_012026Gamemode::SpawnNaves()
{
    UWorld* World = GetWorld();
    if (!World) return;

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

        ANave* NewNave = World->SpawnActor<ANave>(ANave::StaticClass(), SpawnLoc, SpawnRot, SpawnParams);
        if (NewNave)
        {
            NavesContenedor.Add(NewNave);
        }
    }
    UE_LOG(LogTemp, Warning, TEXT("20 naves con patrones de movimiento spawneadas."));
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
    FVector BaseTarget = PawnLocation + PawnForward * 500.0f;

    int32 Columnas = 5;
    int32 Filas = 4;
    float EspaciadoH = 150.0f;
    float EspaciadoV = 150.0f;

    int32 index = 0;
    for (int32 fila = 0; fila < Filas; ++fila)
    {
        for (int32 col = 0; col < Columnas; ++col)
        {
            if (index >= NavesContenedor.Num()) break;

            float OffsetX = (col - Columnas / 2) * EspaciadoH;
            float OffsetY = (fila - Filas / 2) * EspaciadoV;
            FVector Target = BaseTarget + PawnRight * OffsetX + FVector(0.0f, 0.0f, OffsetY);
            Target.Z = PawnLocation.Z;

            NavesContenedor[index]->StartFormation(Target);
            ++index;
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("Naves en formación frente al Pawn."));
    GetWorldTimerManager().SetTimer(TimerRetorno, this,
        &ANavesUSFX_012026Gamemode::ReturnNavesAutonomous,
        5.0f, false);
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

// ========== Método Lab 4 (Laberinto con muros delgados pegados, entrada y salida) ==========
void ANavesUSFX_012026Gamemode::GenerarLaberintoConBloques()
{
    UWorld* World = GetWorld();
    if (!World) return;

    const int32 Filas = 17;
    const int32 Columnas = 16;
    // Laberinto con pasillos de 150 unidades, entrada y salida externas
    int32 Laberinto[Filas][Columnas] =
    {
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,1},   // entrada amplia (3 celdas libres)
        {0,0,1,0,1,0,1,1,1,1,0,1,0,1,0,1},
        {0,0,1,0,0,0,0,0,0,1,0,0,0,1,0,1},
        {1,0,1,1,1,1,1,1,0,1,1,1,1,1,0,1},
        {1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1},
        {1,0,1,1,1,1,0,1,0,1,1,1,1,1,0,1},
        {1,0,1,0,0,0,0,1,0,1,0,0,0,0,0,1},
        {1,0,1,0,1,1,1,1,0,1,0,1,1,1,0,1},
        {1,0,0,0,0,0,0,0,0,1,0,0,0,1,0,1},   // zona libre donde aparece el jugador
        {1,1,1,1,1,1,1,1,0,1,1,1,0,1,0,1},
        {1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1},
        {1,0,1,1,1,1,1,1,0,1,0,1,1,1,0,1},
        {1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1},
        {1,1,1,1,1,1,0,1,1,1,1,1,1,1,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},   // salida amplia (3 celdas libres)
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
    };

    // Tamaño de celda y offsets adaptados a celdas de 150
    const float TamanoCelda = 150.0f;       // pasillo de 150 unidades
    const float OffsetX = -1200.0f;         // 16 * 150 / 2 = 1200
    const float OffsetY = -1275.0f;         // 17 * 150 / 2 = 1275
    const float AlturaZ = 220.0f;

    // Generar bloques (muros) con escala 1.5 para llenar las celdas de 150
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

                ABloque* Bloque = World->SpawnActor<ABloque>(ABloque::StaticClass(), Pos, Rot, SpawnParams);
                if (Bloque)
                {
                    // Escala 1.5 para que el cubo ocupe 150x150x150
                    Bloque->MeshComp->SetWorldScale3D(FVector(1.5f, 1.5f, 1.0f));
                    ListaBloquesLaberinto.insertar(Bloque);
                }
            }
        }
    }

    // Función para obtener el centro de una celda
    auto CentroCelda = [&](int32 Col, int32 Fila) -> FVector
    {
        return FVector(OffsetX + Col * TamanoCelda + TamanoCelda * 0.5f,
            OffsetY + Fila * TamanoCelda + TamanoCelda * 0.5f,
            AlturaZ);
    };

    // Ruta con entrada y salida externas
    TArray<FVector> RutaLaberinto;

    // Punto externo izquierdo (entrada)
    FVector EntradaExterna = CentroCelda(1, 1) - FVector(100.0f, 0.0f, 0.0f);
    RutaLaberinto.Add(EntradaExterna);

    // Recorrido interno (los índices son los mismos, ahora con celdas de 150)
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
    RutaLaberinto.Add(CentroCelda(15, 15)); // salida interna

    // Punto externo derecho (salida)
    FVector SalidaExterna = CentroCelda(15, 15) + FVector(100.0f, 0.0f, 0.0f);
    RutaLaberinto.Add(SalidaExterna);

    // Patrulla de 3 drones
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

    UE_LOG(LogTemp, Warning, TEXT("Laberinto con pasillos de 150 y entrada/salida amplias generado."));
}