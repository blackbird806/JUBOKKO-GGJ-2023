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

void ATreeNode::Init(ATree* inTree, ATreeNode* inPrev, FVector Pos)
{
	check(inTree);
	Tree = inTree;
	Prev = inPrev;
	bIsdead = false;

	Tree->BloodAmount -= Tree->BloodLost;
	GEngine->AddOnScreenDebugMessage(-1, 200, FColor::Green, FString::Printf(TEXT("BloodAmount %f"), Tree->BloodAmount));
	if (Tree->BloodAmount <= 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 200, FColor::Red, TEXT("GameOver"));
	}

	FVector2D ScreenLocation;
	if (GetWorld()->GetFirstPlayerController()->ProjectWorldLocationToScreen(Pos, ScreenLocation))
	{
		Pos2D = ScreenLocation;
	}

	RootComponent = NewObject<USceneComponent>(this);
	SetActorLocation(Pos);

	Tree->Nodes.Add(this);

	auto* NodeMesh = GetWorld()->SpawnActor<ATreeRootNode>();
	auto* ActorComp = NodeMesh->AddComponentByClass(UStaticMeshComponent::StaticClass(), false, FTransform{}, false);
	UStaticMeshComponent* MeshComp = Cast<UStaticMeshComponent>(ActorComp);
	MeshComp->SetStaticMesh(Tree->NodeMeshs[FMath::RandRange(0, Tree->NodeMeshs.Num() - 1)]);
	NodeMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	NodeMesh->SetActorHiddenInGame(false);
	NodeMesh->SetActorScale3D(FVector(0.2f, 0.2f, 0.2f));
	NodeMesh->SetActorLocation(Pos);

	if (!IsRoot())
	{
		Prev->Next.Add(this);
		PipeMesh = GetWorld()->SpawnActor<ATreeRootNode>();
		PipeMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		auto* ActorPipeComp = Cast<UStaticMeshComponent>(PipeMesh->AddComponentByClass(UStaticMeshComponent::StaticClass(), false, FTransform{}, false));
		ActorPipeComp->SetStaticMesh(Tree->PipeMeshs[FMath::RandRange(0, Tree->PipeMeshs.Num() - 1)]);
		PipeMesh->SetActorHiddenInGame(false);

		UpdateMesh();
	}

	// call vfx
}

void ATreeNode::UpdateMesh()
{
	PipeMesh->SetActorScale3D(FVector(FVector::Dist(GetActorLocation(), Prev->GetActorLocation()) / 1000.0f, 0.1f, 0.1f));
	auto const PipeLoc = (Prev->GetActorLocation() + ((GetActorLocation() - Prev->GetActorLocation()) / 2.0f));
	PipeMesh->SetActorLocation(PipeLoc);
	PipeMesh->SetActorRotation((GetActorLocation() - Prev->GetActorLocation()).Rotation());

	Distance = FVector::Dist(Prev->GetActorLocation(), GetActorLocation());
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

	//DrawDebugPoint(GetWorld(), GetActorLocation(), 5.0, FColor::Blue);
}

