#include "TreeNode.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/PlayerController.h"
#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
ATreeNode::ATreeNode()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ATreeNode::Init(ATree* inTree, ATreeNode* inPrev, FVector2D Pos)
{
	check(inTree);
	Tree = inTree;
	Prev = inPrev;
	bIsdead = false;
	Pos2D = Pos;

	RootComponent = NewObject<USceneComponent>(this);

	FVector WorldLocation;
	FVector WorldDirection;
	if (Tree->DeprojectScreenPositionToWorld(Pos2D.X, Pos2D.Y, WorldLocation, WorldDirection))
	{
		SetActorLocation(WorldLocation);
	}

	Tree->Nodes.Add(this);
	FSplinePoint pt{};
	static float inputkey = 1.0f;
	inputkey += 0.2f;
	pt.InputKey = inputkey;
	pt.Position = GetActorLocation();
	Tree->Spline->AddPoint(pt);
	if (!IsRoot())
	{
		Prev->Next.Add(this);
		SplineMesh = NewObject<USplineMeshComponent>(this);
		ensure(SplineMesh);
		SplineMesh->SetStaticMesh(Tree->SplineMesh);
		SplineMesh->SetStartScale(FVector2D(0.1, 0.1));
		SplineMesh->SetEndScale(FVector2D(0.1, 0.1));
		SplineMesh->SplineParams.StartPos = Prev->GetActorLocation();
		SplineMesh->SplineParams.StartTangent = /*Tree->Tangent1;  //*/Tree->Spline->GetTangentAtSplinePoint(Tree->Spline->GetNumberOfSplinePoints() - 1, ESplineCoordinateSpace::Local);
		SplineMesh->SplineParams.EndPos = GetActorLocation();
		SplineMesh->SplineParams.EndTangent = /*Tree->Tangent2; //*/ Tree->Spline->GetTangentAtSplinePoint(Tree->Spline->GetNumberOfSplinePoints(), ESplineCoordinateSpace::Local);
		SplineMesh->RegisterComponentWithWorld(GetWorld());
		SplineMesh->CreationMethod = EComponentCreationMethod::UserConstructionScript;
		SplineMesh->SetMobility(EComponentMobility::Movable);
		SplineMesh->SetForwardAxis(ESplineMeshAxis::Y);

		SplineMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		Distance = FVector::Dist(Prev->GetActorLocation(), GetActorLocation());

		FVector Tangent = (Prev->GetActorLocation() - GetActorLocation());
		Tangent.Normalize();
		if (!Prev->IsRoot())
		{
			Prev->SplineMesh->SetStartTangent(Tangent);
		}
		//SplineMesh->SetEndTangent(Tangent);
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
	if (!IsRoot())
	{
		//SplineMesh->SplineParams.EndPos = GetActorLocation();
	}
}

