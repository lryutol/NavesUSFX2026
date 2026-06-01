#include "EADron.h"

AEADron::AEADron()
{
    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("/Engine/BasicShapes/Capsule"));
    if (MeshObj.Succeeded())
    {
        MeshComponent->SetStaticMesh(MeshObj.Object);
        MeshComponent->SetWorldScale3D(FVector(0.4f, 0.4f, 0.4f));
    }
}