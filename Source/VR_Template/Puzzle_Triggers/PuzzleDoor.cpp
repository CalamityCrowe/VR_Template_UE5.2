// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzleDoor.h"
#include "Components/TimelineComponent.h"
// Sets default values
APuzzleDoor::APuzzleDoor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ClosedRotation = FRotator(0, 0, 0);
	OpenRotation = FRotator(90, 0, 0);

	auto CurveAsset = ConstructorHelpers::FObjectFinder<UCurveFloat>(TEXT("CurveFloat'/Game/Data_Tables/Curves/DoorLerpCurve.DoorLerpCurve'"));
	DoorTimeCurve = CurveAsset.Object;

}

// Called when the game starts or when spawned
void APuzzleDoor::BeginPlay()
{
	Super::BeginPlay();

	if(DoorTimeCurve != NULL)
	{
		DoorTimeline = NewObject<UTimelineComponent>(this, FName("TimelineAnimation"));
		DoorTimeline->CreationMethod = EComponentCreationMethod::UserConstructionScript;
		this->BlueprintCreatedComponents.Add(DoorTimeline);
		DoorTimeline->SetNetAddressable();

		DoorTimeline->SetPropertySetObject(this);
		DoorTimeline->SetDirectionPropertyName(FName("TimelineDirection"));

		DoorTimeline->SetLooping(false);
		DoorTimeline->SetTimelineLength(5.0f);
		DoorTimeline->SetTimelineLengthMode(ETimelineLengthMode::TL_LastKeyFrame);

		DoorTimeline->SetPlaybackPosition(0.0f, false);

		

	}

}

// Called every frame
void APuzzleDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APuzzleDoor::OpenDoor()
{
	FTimerHandle time;

}

