#include "EAAvion.h"

AEAAvion::AEAAvion()
{
    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("/Engine/BasicShapes/Cone"));
    if (MeshObj.Succeeded())
    {
        MeshComponent->SetStaticMesh(MeshObj.Object);
        // Le damos la vuelta para que parezca un avión
        MeshComponent->SetWorldRotation(FRotator(180.0f, 0.0f, 0.0f));
        MeshComponent->SetWorldScale3D(FVector(1.0f, 1.0f, 2.0f));
    }
}