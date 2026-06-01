#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "NavesUSFX_012026Gamemode.generated.h"

class AEnemigo;

UCLASS()
class NAVESUSFX_012026_API ANavesUSFX_012026Gamemode : public AGameModeBase
{
    GENERATED_BODY()

public:
    ANavesUSFX_012026Gamemode();

protected:
    virtual void BeginPlay() override;

private:
    // Primera cuadrilla
    TArray<TSubclassOf<AEnemigo>> Squad1Classes;
    TArray<FVector> Squad1Locations;
    TArray<FVector> Squad1Waypoints;

    // Segunda cuadrilla
    TArray<TSubclassOf<AEnemigo>> Squad2Classes;
    TArray<FVector> Squad2Locations;
    TArray<FVector> Squad2Waypoints;

    // Referencias a los enemigos vivos de la primera cuadrilla
    TArray<TWeakObjectPtr<AEnemigo>> Squad1Members;

    // Temporizador para verificar desaparición
    FTimerHandle CheckSquad1Timer;

    void SpawnSquad(const TArray<TSubclassOf<AEnemigo>>& Classes,
        const TArray<FVector>& Locations,
        const TArray<FVector>& Waypoints,
        TArray<TWeakObjectPtr<AEnemigo>>& OutMembers);

    void CheckSquad1Status();
    void SpawnSquad2();
};