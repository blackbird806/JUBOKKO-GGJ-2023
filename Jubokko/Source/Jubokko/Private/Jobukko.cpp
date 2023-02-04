// Fill out your copyright notice in the Description page of Project Settings.


#include "Jobukko.h"

// Sets default values
AJobukko::AJobukko()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AJobukko::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AJobukko::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AJobukko::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

