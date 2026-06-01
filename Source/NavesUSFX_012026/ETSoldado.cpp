#include "ETSoldado.h"

AETSoldado::AETSoldado()
{
    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("/Engine/BasicShapes/Capsule"));
    if (MeshObj.Succeeded())
    {
        MeshComponent->SetStaticMesh(MeshObj.Object);
        MeshComponent->SetWorldScale3D(FVector(0.6f, 0.6f, 1.8f));
    }
}