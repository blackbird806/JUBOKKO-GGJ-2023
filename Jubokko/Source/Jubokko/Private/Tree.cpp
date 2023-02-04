#include "Tree.h"

#include "TreeNode.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

ATree::ATree()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ATree::BeginPlay()
{
	Super::BeginPlay();
	InputComponent->BindAction("MouseLeft", IE_Pressed, this, &ATree::MousePress);
	InputComponent->BindAction("MouseLeft", IE_Released, this, &ATree::MouseRelease);
	Last = GetWorld()->SpawnActor<ATreeNode>();
	Spline = FindComponentByClass<USplineComponent>();
	check(Spline);
	Last->Init(this, nullptr, FVector2D{800, 500});
}

// Called every frame
void ATree::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetMousePosition(MousePos.X, MousePos.Y);

	if (bIsMouseLeftPressed)
	{
		Timer += DeltaTime;
		if (Timer > TimeInterval)
		{
			Timer = 0.0f;
			ATreeNode* Node = GetWorld()->SpawnActor<ATreeNode>();
			Node->Init(this, Last, MousePos);
			Last = Node;
		}
	}
}

void ATree::MousePress()
{
	float MinDist = FLT_MAX;
	for (ATreeNode* Node : Nodes)
	{
		float const Dist = FVector2D::DistSquared(Node->Pos2D, MousePos);
		if (Dist < SelectionRange && Dist < MinDist)
		{
			bIsMouseLeftPressed = true;
			MinDist = Dist;
			Last = Node;
		}
	}
}

void ATree::MouseRelease()
{
	bIsMouseLeftPressed = false;
}
