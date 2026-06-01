#include "EALancha.h"

AEALancha::AEALancha()
{
    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("/Engine/BasicShapes/Cylinder"));
    if (MeshObj.Succeeded())
    {
        MeshComponent->SetStaticMesh(MeshObj.Object);
        MeshComponent->SetWorldRotation(FRotator(0.0f, 0.0f, 90.0f));
        MeshComponent->SetWorldScale3D(FVector(0.5f, 0.5f, 2.0f));
    }
}