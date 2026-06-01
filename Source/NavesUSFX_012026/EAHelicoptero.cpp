#include "EAHelicoptero.h"

AEAHelicoptero::AEAHelicoptero()
{
    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("/Engine/BasicShapes/Cylinder"));
    if (MeshObj.Succeeded())
    {
        MeshComponent->SetStaticMesh(MeshObj.Object);
        MeshComponent->SetWorldScale3D(FVector(0.7f, 0.7f, 1.5f));
    }
}