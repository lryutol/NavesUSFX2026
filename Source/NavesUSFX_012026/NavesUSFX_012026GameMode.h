#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "NavesUSFX_012026Gamemode.generated.h"

UCLASS()
class NAVESUSFX_012026_API ANavesUSFX_012026Gamemode : public AGameModeBase
{
    GENERATED_BODY()

public:
    ANavesUSFX_012026Gamemode();

protected:
    virtual void BeginPlay() override;

private:
    TSubclassOf<class AEnemigo> EnemyClass;
    TArray<FVector> SpawnLocations;
    TArray<FVector> GlobalWaypoints;
    int32 NumEnemies;
};