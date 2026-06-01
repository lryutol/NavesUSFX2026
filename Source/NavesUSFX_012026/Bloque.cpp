#include "Bloque.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/StaticMesh.h"

ABloque::ABloque()
{
    MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
    RootComponent = MeshComp;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cube"));
    if (CubeMesh.Succeeded())
    {
        MeshComp->SetStaticMesh(CubeMesh.Object);
        MeshComp->SetWorldScale3D(FVector(0.5f));
    }
}