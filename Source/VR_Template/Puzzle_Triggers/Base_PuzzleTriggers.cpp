// Fill out your copyright notice in the Description page of Project Settings.


#include "Base_PuzzleTriggers.h"
#include "Components/BoxComponent.h"

// Sets default values
ABase_PuzzleTriggers::ABase_PuzzleTriggers()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	m_SceneRoot = CreateOptionalDefaultSubobject<USceneComponent>(TEXT("Scene Root")); 
	RootComponent = m_SceneRoot; 

	m_Collider = CreateOptionalDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	m_Collider->SetupAttachment(m_SceneRoot); 
	m_Collider->SetBoxExtent(FVector(100, 100, 100), true); 
	m_Collider->SetLineThickness(2.5f); 
	m_Collider->ShapeColor = FColor(128, 0, 255); 

}

// Called when the game starts or when spawned
void ABase_PuzzleTriggers::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABase_PuzzleTriggers::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(278, 2, FColor(255, 128, 255), TEXT("Puzzle Trigger Overlap Activated")); 
}

// Called every frame
void ABase_PuzzleTriggers::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

