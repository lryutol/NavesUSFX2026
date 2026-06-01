#include "ETSoldado.h"

AETSoldado::AETSoldado()
{
    // Cilindro vertical alto (soldado)
    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("/Engine/BasicShapes/Cylinder"));
    if (MeshObj.Succeeded())
    {
        MeshComponent->SetStaticMesh(MeshObj.Object);
        MeshComponent->SetWorldScale3D(FVector(0.6f, 0.6f, 1.8f));
    }
}