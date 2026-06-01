#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "NavesUSFX_012026Gamemode.generated.h"

class UNaveManager;

UCLASS()
class NAVESUSFX_012026_API ANavesUSFX_012026Gamemode : public AGameModeBase
{
    GENERATED_BODY()

public:
    ANavesUSFX_012026Gamemode();

protected:
    virtual void BeginPlay() override;

private:
    UNaveManager* NaveManager;
};