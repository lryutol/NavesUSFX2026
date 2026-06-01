#include "EnemigoAcuatico.h"

AEnemigoAcuatico::AEnemigoAcuatico()
{
    // Malla acuática: esfera grande
    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("/Engine/BasicShapes/Sphere"));
    if (MeshObj.Succeeded())
    {
        MeshComponent->SetStaticMesh(MeshObj.Object);
        MeshComponent->SetWorldScale3D(FVector(1.5f));
    }
    Lifespan = 10.0f;
}