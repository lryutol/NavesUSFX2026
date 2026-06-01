#include "EABarco.h"

AEABarco::AEABarco()
{
    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("/Engine/BasicShapes/Cone"));
    if (MeshObj.Succeeded())
    {
        MeshComponent->SetStaticMesh(MeshObj.Object);
        MeshComponent->SetWorldScale3D(FVector(2.0f, 1.5f, 0.5f));
    }
}