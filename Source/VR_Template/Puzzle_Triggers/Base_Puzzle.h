// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Base_Puzzle.generated.h"

DECLARE_DELEGATE(FOnPuzzleCompletedDelegate)


UCLASS()
class VR_TEMPLATE_API ABase_Puzzle : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABase_Puzzle();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void PuzzleCompleteSignal();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FOnPuzzleCompletedDelegate OnPuzzleCompletedDelegate;

	void DoOnce() { if (bCallOnce == false) { bCallOnce = true; bDoOnce = true; } }

private:

	bool bCallOnce = false;

	bool bDoOnce;



public:
	

};
