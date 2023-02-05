#pragma once

#include "CoreMinimal.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "GameFramework/PlayerController.h"
#include "Tree.generated.h"

UCLASS()
class ATreeRootNode : public AActor
{
	GENERATED_BODY()
};

UCLASS()
class JUBOKKO_API ATree : public APawn
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BloodAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BloodLost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BloodGain;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int NbKills;

	TArray<class ATreeNode*> Nodes;
	ATreeNode* Last = nullptr;
	FVector2D MousePos;
	bool bIsMouseLeftPressed = false;
	float Timer = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* RootSpawnPosition;

	UPROPERTY(EditAnywhere)
	TArray<UStaticMesh*> PipeMeshs;

	UPROPERTY(EditAnywhere)
	TArray<UStaticMesh*> NodeMeshs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ProjectionLength =100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TimeInterval = 0.3f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SelectionRange = 5.0f;


	ATree();

protected:
	virtual void BeginPlay() override;

public:
	FVector GetNodeLocationFromMouse(ATreeNode* Connected);
	virtual void Tick(float DeltaTime) override;
	void MoveX(float Vec);
	void MoveY(float Vec);
	void MousePress();
	void MouseRelease();
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintImplementableEvent)
	void OnKillNPC(AActor* Actor);

	UFUNCTION(BlueprintCallable)
	ATreeNode* IsANodeInThisFloor(int inFloor);
};
