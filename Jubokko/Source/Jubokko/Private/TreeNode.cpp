#include "TreeNode.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/PlayerController.h"
#include "Engine/Engine.h"

// Sets default values
ATreeNode::ATreeNode()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ATreeNode::Init(ATree* inTree, ATreeNode* inPrev, FVector2D Pos)
{
	check(inTree);
	check(inPrev);
	Tree = inTree;
	Prev = inPrev;
	bIsdead = false;
	Pos2D = Pos;

	Tree->Nodes.Add(this);
	Prev->Next.Add(this);

	Distance = FVector::Dist(Prev->GetActorLocation(), GetActorLocation());
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

	FVector WorldLocation;
	FVector WorldDirection;

	if (PlayerController->DeprojectScreenPositionToWorld(Pos2D.X, Pos2D.Y, WorldLocation, WorldDirection))
	{
		DrawDebugPoint(GetWorld(), WorldLocation, 5.0, FColor::Blue);
	}
}

