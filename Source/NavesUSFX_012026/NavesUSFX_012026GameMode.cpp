#include "NavesUSFX_012026Gamemode.h"
#include "Enemigo.h"
// Incluir todas las clases concretas que usaremos
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

ANavesUSFX_012026Gamemode::ANavesUSFX_012026Gamemode()
{
    // ================== PRIMERA CUADRILLA ==================
    // Combinación de aéreos y terrestres
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

    // ================== SEGUNDA CUADRILLA ==================
    // Combinación de acuáticos y un camión
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

    // Spawn de la primera cuadrilla
    SpawnSquad(Squad1Classes, Squad1Locations, Squad1Waypoints, Squad1Members);

    // Temporizador que revisa cada segundo si la primera cuadrilla desapareció
    GetWorldTimerManager().SetTimer(CheckSquad1Timer,
        this,
        &ANavesUSFX_012026Gamemode::CheckSquad1Status,
        1.0f,
        true);
}

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
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

        AEnemigo* NewEnemy = World->SpawnActor<AEnemigo>(Classes[i], Loc, Rot, SpawnParams);
        if (NewEnemy)
        {
            NewEnemy->InitializeWaypoints(Waypoints);
            OutMembers.Add(NewEnemy);
            UE_LOG(LogTemp, Log, TEXT("Enemigo spawneado en %s"), *Loc.ToString());
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
        // Detener el temporizador
        GetWorldTimerManager().ClearTimer(CheckSquad1Timer);

        UE_LOG(LogTemp, Warning, TEXT("Primera cuadrilla eliminada. Aparece segunda cuadrilla."));
        SpawnSquad2();
    }
}

void ANavesUSFX_012026Gamemode::SpawnSquad2()
{
    TArray<TWeakObjectPtr<AEnemigo>> Squad2Members; // no se usan después
    SpawnSquad(Squad2Classes, Squad2Locations, Squad2Waypoints, Squad2Members);
}