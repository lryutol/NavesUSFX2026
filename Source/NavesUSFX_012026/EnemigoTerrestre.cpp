#include "EnemigoTerrestre.h"

AEnemigoTerrestre::AEnemigoTerrestre()
{
    // Malla terrestre: cubo achatado
    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("/Engine/BasicShapes/Cube"));
    if (MeshObj.Succeeded())
    {
        MeshComponent->SetStaticMesh(MeshObj.Object);
        MeshComponent->SetWorldScale3D(FVector(1.5f, 1.5f, 0.5f));
    }
    Lifespan = 10.0f;
}