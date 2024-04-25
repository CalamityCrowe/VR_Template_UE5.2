// Fill out your copyright notice in the Description page of Project Settings.


#include "Base_PuzzleTriggers.h"
#include "Components/BoxComponent.h"
#include "Cordelias_Sight/Interactable/Base_Interactable.h"

// Sets default values
ABase_PuzzleTriggers::ABase_PuzzleTriggers()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SceneRoot = CreateOptionalDefaultSubobject<USceneComponent>(TEXT("Scene Root"));
	RootComponent = SceneRoot;

	Collider = CreateOptionalDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	Collider->SetupAttachment(SceneRoot);
	Collider->SetBoxExtent(FVector(100, 100, 100), true);
	Collider->SetLineThickness(2.5f);
	Collider->ShapeColor = FColor(128, 0, 255);

	TargetToCatch = 3; // this is the target for the number of objects to get into the puzzle

}

// Called when the game starts or when spawned
void ABase_PuzzleTriggers::BeginPlay()
{
	Super::BeginPlay();

}

void ABase_PuzzleTriggers::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
#if WITH_EDITOR
	GEngine->AddOnScreenDebugMessage(278, 2, FColor(255, 128, 255), TEXT("Puzzle Trigger Overlap Activated"));
#endif

	if (ABase_Interactable* ThrowActor = Cast<ABase_Interactable>(OtherActor))
	{
		if (ThrowActor->GetObjectType() == EObjectType::Throwable)
		{
			NumberCaught++;
			if (NumberCaught == TargetToCatch)
			{
				// do the stuff to trigger to go to the next puzzle
			}
		}
	}
}

void ABase_PuzzleTriggers::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (ABase_Interactable* ThrowActor = Cast<ABase_Interactable>(OtherActor))
	{
		if (ThrowActor->GetObjectType() == EObjectType::Throwable)
		{
			NumberCaught--;
		}
	}
}

// Called every frame
void ABase_PuzzleTriggers::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

