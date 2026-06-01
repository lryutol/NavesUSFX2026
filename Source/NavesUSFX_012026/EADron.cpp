#include "EADron.h"

AEADron::AEADron()
{
    // Usamos un cilindro aplanado (simula un dron pequeño)
    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("/Engine/BasicShapes/Cylinder"));
    if (MeshObj.Succeeded())
    {
        MeshComponent->SetStaticMesh(MeshObj.Object);
        MeshComponent->SetWorldScale3D(FVector(0.4f, 0.4f, 0.2f));
    }
}