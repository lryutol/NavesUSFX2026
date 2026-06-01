#include "NavesUSFX_012026Gamemode.h"
#include "Enemigo.h"
#include "Engine/World.h"

ANavesUSFX_012026Gamemode::ANavesUSFX_012026Gamemode()
{
    EnemyClass = AEnemigo::StaticClass();
    NumEnemies = 4;

    // Posiciones de spawn ajustadas (X, Y, Z) ajustando a la altura del jugador
    SpawnLocations.Add(FVector(200, 0, 214));
    SpawnLocations.Add(FVector(0, 200, 214));
    SpawnLocations.Add(FVector(-200, 0, 214));
    SpawnLocations.Add(FVector(0, -200, 214));

    // Waypoints globales también a nivel del jugador Z
    GlobalWaypoints.Add(FVector(500, 0, 214));
    GlobalWaypoints.Add(FVector(0, 500, 214));
    GlobalWaypoints.Add(FVector(-500, 0, 214));
    GlobalWaypoints.Add(FVector(0, -500, 214));
}

void ANavesUSFX_012026Gamemode::BeginPlay()
{
    Super::BeginPlay();

    if (!EnemyClass) return;

    UWorld* World = GetWorld();
    if (!World) return;

    for (int32 i = 0; i < NumEnemies; i++)
    {
        FVector Location = SpawnLocations.IsValidIndex(i) ? SpawnLocations[i] : FVector(FMath::RandRange(-300, 300), FMath::RandRange(-300, 300), 150);
        FRotator Rotation = FRotator::ZeroRotator;
        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

        AEnemigo* NewEnemy = World->SpawnActor<AEnemigo>(EnemyClass, Location, Rotation, SpawnParams);
        if (NewEnemy)
        {
            NewEnemy->InitializeWaypoints(GlobalWaypoints);
            UE_LOG(LogTemp, Log, TEXT("Enemigo %d generado en %s"), i, *Location.ToString());
        }
    }
}