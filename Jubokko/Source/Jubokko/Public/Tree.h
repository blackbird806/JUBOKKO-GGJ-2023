#pragma once

#include "CoreMinimal.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "GameFramework/PlayerController.h"
#include "Tree.generated.h"

UCLASS()
class JUBOKKO_API ATree : public APlayerController
{
	GENERATED_BODY()

public:
	TArray<class ATreeNode*> Nodes;
	ATreeNode* Last = nullptr;
	FVector2D MousePos;
	bool bIsMouseLeftPressed = false;
	float Timer = 0;

	UPROPERTY(EditAnywhere)
		UStaticMesh* PipeMesh;

	UPROPERTY(EditAnywhere)
	UStaticMesh* NodeMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TimeInterval = 0.3f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SelectionRange = 5.0f;

	ATree();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	void MoveX(float Vec);
	void MoveY(float Vec);
	void MousePress();
	void MouseRelease();

};
