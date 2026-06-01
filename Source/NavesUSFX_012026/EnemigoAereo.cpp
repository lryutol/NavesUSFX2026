#include "EnemigoAereo.h"

AEnemigoAereo::AEnemigoAereo()
{
    // Malla aérea: cono alargado
    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("/Engine/BasicShapes/Cone"));
    if (MeshObj.Succeeded())
    {
        MeshComponent->SetStaticMesh(MeshObj.Object);
        MeshComponent->SetWorldScale3D(FVector(1.0f, 1.0f, 2.0f));
    }
    Lifespan = 10.0f;
}