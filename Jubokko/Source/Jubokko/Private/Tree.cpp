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
	GetWorld()->GetFirstPlayerController()->SetControlRotation(FRotator());
	Last = GetWorld()->SpawnActor<ATreeNode>();
	FVector2D ScreenLoation;
	if (GetWorld()->GetFirstPlayerController()->ProjectWorldLocationToScreen(RootSpawnPosition->GetActorLocation(), ScreenLoation))
	{
	}
	Last->Init(this, nullptr, ScreenLoation, RootSpawnPosition->GetActorLocation());
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
			FVector WorldLocation;
			FVector WorldDirection;
			if (GetWorld()->GetFirstPlayerController()->DeprojectScreenPositionToWorld(MousePos.X, MousePos.Y, WorldLocation, WorldDirection))
			{
				FHitResult Hinfo;
				WorldLocation += WorldDirection * ProjectionLength;
				if (GetWorld()->LineTraceSingleByChannel(Hinfo, Last->GetActorLocation(), WorldLocation, ECC_Visibility))
				{
					WorldLocation = Hinfo.ImpactPoint;
				}
			}
			Node->Init(this, Last, MousePos, WorldLocation);
			Last = Node;
		}
	}
}

void ATree::MousePress()
{
	float MinDist = FLT_MAX;
	for (ATreeNode* Node : Nodes)
	{
		float const Dist = FVector2D::Distance(Node->Pos2D, MousePos);
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

void ATree::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("MouseLeft", IE_Pressed, this, &ATree::MousePress);
	PlayerInputComponent->BindAction("MouseLeft", IE_Released, this, &ATree::MouseRelease);
}
