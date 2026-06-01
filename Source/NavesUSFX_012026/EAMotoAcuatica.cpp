#include "EAMotoAcuatica.h"

AEAMotoAcuatica::AEAMotoAcuatica()
{
    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("/Engine/BasicShapes/Capsule"));
    if (MeshObj.Succeeded())
    {
        MeshComponent->SetStaticMesh(MeshObj.Object);
        MeshComponent->SetWorldRotation(FRotator(0.0f, 90.0f, 0.0f));
        MeshComponent->SetWorldScale3D(FVector(0.6f, 1.2f, 0.6f));
    }
}