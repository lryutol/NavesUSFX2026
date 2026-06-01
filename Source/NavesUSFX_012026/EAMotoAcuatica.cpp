#include "EAMotoAcuatica.h"

AEAMotoAcuatica::AEAMotoAcuatica()
{
    // Cilindro alargado horizontal (moto acuática)
    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("/Engine/BasicShapes/Cylinder"));
    if (MeshObj.Succeeded())
    {
        MeshComponent->SetStaticMesh(MeshObj.Object);
        MeshComponent->SetWorldRotation(FRotator(0.0f, 90.0f, 0.0f));
        MeshComponent->SetWorldScale3D(FVector(0.6f, 1.2f, 0.6f));
    }
}