// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tree.h"
#include "TreeNode.generated.h"

UCLASS()
class JUBOKKO_API ATreeNode : public AActor
{
	GENERATED_BODY()
	
public:

	UPROPERTY()
	ATree* Tree;
	UPROPERTY(BlueprintReadWrite)
	ATreeNode* Prev;
	UPROPERTY(BlueprintReadWrite)
	TArray<ATreeNode*> Next;
	UPROPERTY(BlueprintReadWrite)
	bool bIsdead;
	UPROPERTY(BlueprintReadWrite)
	float Distance;
	UPROPERTY(BlueprintReadWrite)
	int Floor;
	UPROPERTY(BlueprintReadWrite)
	int Life = 3;

	FVector2D Pos2D;

	static constexpr float KillLatency = 0.2f;

	FTimerHandle KillTimer;

	UPROPERTY(BlueprintReadWrite)
	AActor* NodeMesh;

	UPROPERTY(BlueprintReadWrite)
	AActor* PipeMesh;

	// Sets default values for this actor's properties
	ATreeNode();
	void Init(ATree* inTree, ATreeNode* inPrev, FVector Pos);

	void UpdateMesh();

	bool IsRoot() const;
	void Kill();
	void Kill2();
	void KillVisuals();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
