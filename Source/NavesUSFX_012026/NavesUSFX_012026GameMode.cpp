#include "NavesUSFX_012026Gamemode.h"
#include "NaveManager.h"

ANavesUSFX_012026Gamemode::ANavesUSFX_012026Gamemode()
{
}

void ANavesUSFX_012026Gamemode::BeginPlay()
{
    Super::BeginPlay();

    NaveManager = NewObject<UNaveManager>(this);
    NaveManager->Inicializar(GetWorld());
}