#include "ETBlindado.h"

AETBlindado::AETBlindado()
{
    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("/Engine/BasicShapes/Cube"));
    if (MeshObj.Succeeded())
    {
        MeshComponent->SetStaticMesh(MeshObj.Object);
        MeshComponent->SetWorldRotation(FRotator(10.0f, 15.0f, 0.0f));
        MeshComponent->SetWorldScale3D(FVector(1.8f, 1.8f, 0.9f));
    }
}