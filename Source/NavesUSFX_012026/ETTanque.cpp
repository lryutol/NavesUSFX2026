#include "ETTanque.h"

AETTanque::AETTanque()
{
    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("/Engine/BasicShapes/Cube"));
    if (MeshObj.Succeeded())
    {
        MeshComponent->SetStaticMesh(MeshObj.Object);
        MeshComponent->SetWorldScale3D(FVector(2.0f, 2.5f, 1.0f));
    }
}