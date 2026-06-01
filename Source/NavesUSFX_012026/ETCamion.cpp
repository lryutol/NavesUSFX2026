#include "ETCamion.h"

AETCamion::AETCamion()
{
    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("/Engine/BasicShapes/Cylinder"));
    if (MeshObj.Succeeded())
    {
        MeshComponent->SetStaticMesh(MeshObj.Object);
        MeshComponent->SetWorldRotation(FRotator(0.0f, 90.0f, 0.0f));
        MeshComponent->SetWorldScale3D(FVector(1.0f, 1.0f, 2.5f));
    }
}