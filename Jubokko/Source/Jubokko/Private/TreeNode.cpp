#include "TreeNode.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/PlayerController.h"
#include "Engine/Engine.h"
#include "Engine/StaticMesh.h"
#include "Engine/StaticMeshActor.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
ATreeNode::ATreeNode()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ATreeNode::Init(ATree* inTree, ATreeNode* inPrev, FVector2D Pos2, FVector Pos)
{
	check(inTree);
	Tree = inTree;
	Prev = inPrev;
	bIsdead = false;
	Pos2D = Pos2;
	FVector WorldLocation;
	FVector WorldDirection;

	RootComponent = NewObject<USceneComponent>(this);
	SetActorLocation(Pos);
	//if (GetWorld()->GetFirstPlayerController()->DeprojectScreenPositionToWorld(Pos.X, Pos.Y, WorldLocation, WorldDirection))
	//{
	//	SetActorLocation(WorldLocation + WorldDirection * Tree->ProjectionLength);
	//}

	Tree->Nodes.Add(this);

	auto* NodeMesh = GetWorld()->SpawnActor<AActor>();
	auto* ActorComp = NodeMesh->AddComponentByClass(UStaticMeshComponent::StaticClass(), false, FTransform{}, false);
	UStaticMeshComponent* MeshComp = Cast<UStaticMeshComponent>(ActorComp);
	MeshComp->SetStaticMesh(Tree->NodeMesh);
	NodeMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	NodeMesh->SetActorHiddenInGame(false);
	NodeMesh->SetActorScale3D(FVector(0.01f, 0.01f, 0.01f));
	auto Loc = NodeMesh->GetActorLocation();
	NodeMesh->SetActorLocation(Loc);

	if (!IsRoot())
	{
		Prev->Next.Add(this);
		auto* PipeMesh = GetWorld()->SpawnActor<AActor>();
		auto* ActorPipeComp = PipeMesh->AddComponentByClass(UStaticMeshComponent::StaticClass(), false, FTransform{}, false);
		UStaticMeshComponent* PipeComp = Cast<UStaticMeshComponent>(ActorPipeComp);
		PipeComp->SetStaticMesh(Tree->PipeMesh);
		PipeMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		PipeMesh->SetActorHiddenInGame(false);
		PipeMesh->SetActorScale3D(FVector(0.005f, 0.005f, FVector::Dist(GetActorLocation(), Prev->GetActorLocation()) / 100.0f));
		auto PipeLoc = (Prev->GetActorLocation() + ((GetActorLocation() - Prev->GetActorLocation()) / 2));
		PipeMesh->SetActorLocation(PipeLoc);
		PipeMesh->SetActorRotation((GetActorLocation() - Prev->GetActorLocation()).Rotation());
		PipeMesh->AddActorLocalRotation(FRotator(90.0, 90.0, 90.0));

		Distance = FVector::Dist(Prev->GetActorLocation(), GetActorLocation());
	}

	// call vfx
}

bool ATreeNode::IsRoot() const
{
	return Prev == nullptr;
}

void ATreeNode::Kill()
{
	bIsdead = true;

	for (ATreeNode* Current : Next)
	{
		Current->Kill();
	}
	KillVisuals();
}

void ATreeNode::KillVisuals()
{
	if (Next.IsEmpty()) // end is reached
	{
		// call vfx
		Prev->Next.Remove(this);
		GetWorld()->GetTimerManager().SetTimer(KillTimer, Prev, &ATreeNode::KillVisuals, KillLatency, false);
		Tree->Nodes.Remove(this);
		Destroy();
	}
}

void ATreeNode::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATreeNode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

	int32 ViewportSizeX, ViewportSizeY;
	PlayerController->GetViewportSize(ViewportSizeX, ViewportSizeY);

	DrawDebugPoint(GetWorld(), GetActorLocation(), 5.0, FColor::Blue);
}

