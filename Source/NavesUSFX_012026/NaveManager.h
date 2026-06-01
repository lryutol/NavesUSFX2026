#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Lista.h"
#include "NaveManager.generated.h"

class ANave;
class AEnemigo;
class ABloque;

UCLASS()
class NAVESUSFX_012026_API UNaveManager : public UObject
{
    GENERATED_BODY()

public:
    void Inicializar(UWorld* World);

private:
    // Lab 2
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

    // Lab 3
    TArray<ANave*> NavesContenedor;
    FTimerHandle TimerFormacion;
    FTimerHandle TimerRetorno;
    void SpawnNaves();
    void FormNavesFrenteAlPawn();
    void ReturnNavesAutonomous();

    // Lab 4
    Lista<ABloque*> ListaBloquesLaberinto;
    void GenerarLaberintoConBloques();

    UWorld* WorldRef;
};