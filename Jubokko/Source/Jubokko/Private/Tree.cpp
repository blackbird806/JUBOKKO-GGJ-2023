#include "Tree.h"

#include "TreeNode.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

ATree::ATree()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ATree::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetFirstPlayerController()->SetControlRotation(FRotator());
	ATreeNode* Root = GetWorld()->SpawnActor<ATreeNode>();
	Root->Init(this, nullptr, RootSpawnPosition->GetActorLocation());
}

FVector ATree::GetNodeLocationFromMouse(ATreeNode* Connected)
{
	FVector WorldLocation;
	FVector WorldDirection;
	if (GetWorld()->GetFirstPlayerController()->DeprojectScreenPositionToWorld(MousePos.X, MousePos.Y, WorldLocation, WorldDirection))
	{
		FHitResult Hinfo;
		WorldLocation = FMath::LinePlaneIntersection(WorldLocation, WorldLocation + WorldDirection * 100000.0f, FPlane(FVector(1.0f, 0.0f, 0.0f), RootSpawnPosition->GetActorLocation().X));
		if (GetWorld()->LineTraceSingleByChannel(Hinfo, Connected->GetActorLocation(), WorldLocation, ECC_WorldStatic))
		{
			AActor* HitActor = Hinfo.GetActor();
			if (HitActor->IsA(ACharacter::StaticClass()))
			{
				OnKillNPC(HitActor);
			}
			else if (!HitActor->IsA(ATreeRootNode::StaticClass()))
			{
				WorldLocation = Hinfo.ImpactPoint;
			}
		}
	}
	return WorldLocation;
}

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
			Node->Init(this, Last, Last->GetActorLocation());
			Last = Node;
		}

		Last->SetActorLocation(GetNodeLocationFromMouse(Last));
		Last->UpdateMesh();
	}
	else
	{
		Timer = 0.0f;
	}

}

void ATree::MousePress()
{
	float MinDist = FLT_MAX;
	ATreeNode* Closest = nullptr;
	for (ATreeNode* Node : Nodes)
	{
		//float const Dist = FVector2D::Distance(Node->Pos2D, MousePos);
		FVector WorldLocation;
		FVector WorldDirection;
		if (GetWorld()->GetFirstPlayerController()->DeprojectScreenPositionToWorld(MousePos.X, MousePos.Y, WorldLocation, WorldDirection))
		{
			WorldLocation = FMath::LinePlaneIntersection(WorldLocation, WorldLocation + WorldDirection * 100000.0f, FPlane(FVector(1.0f, 0.0f, 0.0f), RootSpawnPosition->GetActorLocation().X));
			float const Dist = FVector::Distance(Node->GetActorLocation(), WorldLocation);
			if (Dist < SelectionRange && Dist < MinDist)
			{
				MinDist = Dist;
				Closest = Node;
			}
		}
	}

	if (Closest)
	{
		bIsMouseLeftPressed = true;
		Last = GetWorld()->SpawnActor<ATreeNode>();
		Last->Init(this, Closest, GetNodeLocationFromMouse(Closest));
	}
}

void ATree::MouseRelease()
{
	bIsMouseLeftPressed = false;
	Last = nullptr;
}

void ATree::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("MouseLeft", IE_Pressed, this, &ATree::MousePress);
	PlayerInputComponent->BindAction("MouseLeft", IE_Released, this, &ATree::MouseRelease);
}
