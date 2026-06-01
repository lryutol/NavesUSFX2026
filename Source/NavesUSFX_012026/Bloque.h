#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bloque.generated.h"

UCLASS()
class NAVESUSFX_012026_API ABloque : public AActor
{
    GENERATED_BODY()

public:
    ABloque();
    UStaticMeshComponent* MeshComp;
};