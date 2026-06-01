#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Lista.h"
#include "NavesUSFX_012026Gamemode.generated.h"

class ANave;
class AEnemigo;
class ABloque;

UCLASS()
class NAVESUSFX_012026_API ANavesUSFX_012026Gamemode : public AGameModeBase
{
    GENERATED_BODY()

public:
    ANavesUSFX_012026Gamemode();

protected:
    virtual void BeginPlay() override;

private:
    // ---------- Lab 2 ----------
    TArray<TSubclassOf<AEnemigo>> Squad1Classes;
    TArray<FVector> Squad1Locations;
    TArray<FVector> Squad1Waypoints;
    TArray<TSubclassOf<AEnemigo>> Squad2Classes;
    TArray<FVector> Squad2Locations;
    TArray<FVector> Squad2Waypoints;
    TArray<TWeakObjectPtr<AEnemigo>> Squad1Members;
    FTimerHandle CheckSquad1Timer;
    void SpawnSquad(const TArray<TSubclassOf<AEnemigo>>& Classes,
        const TArray<FVector>& Locations,
        const TArray<FVector>& Waypoints,
        TArray<TWeakObjectPtr<AEnemigo>>& OutMembers);
    void CheckSquad1Status();
    void SpawnSquad2();

    // ---------- Lab 3 ----------
    TArray<ANave*> NavesContenedor;
    void SpawnNaves();
    void FormNavesFrenteAlPawn();
    void ReturnNavesAutonomous();
    FTimerHandle TimerFormacion;
    FTimerHandle TimerRetorno;

    // ---------- Lab 4 ----------
    Lista<ABloque*> ListaBloquesLaberinto;
    void GenerarLaberintoConBloques();
};