// Fill out your copyright notice in the Description page of Project Settings.


#include "Base_Puzzle.h"

// Sets default values
ABase_Puzzle::ABase_Puzzle()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	bIsCompleted = false; 
}

// Called when the game starts or when spawned
void ABase_Puzzle::BeginPlay()
{
	Super::BeginPlay();



}

void ABase_Puzzle::PuzzleCompleteSignal()
{
	if (OnPuzzleCompletedDelegate.IsBound())
	{
		OnPuzzleCompletedDelegate.Execute();
	}
}

// Called every frame
void ABase_Puzzle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(bDoOnce)
	{
		PuzzleCompleteSignal();
		bDoOnce = false; 

	}

}

