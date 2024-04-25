// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomVRLevelScript.h"
#include <Kismet/GameplayStatics.h>

ACustomVRLevelScript::ACustomVRLevelScript()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACustomVRLevelScript::BeginPlay()
{
	Super::BeginPlay();
}

void ACustomVRLevelScript::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACustomVRLevelScript::AddTimeStamp(float TimeStamp)
{
		TimeStamps.Add(TimeStamp);
}

void ACustomVRLevelScript::PrintTotalTime()
{
	if(TimeStamps.Num() <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("No TimeStamps have been logged, exiting function!"));
		return;
	}
	for(size_t i = 0; i < TimeStamps.Num(); ++i)
	{
		TotalTime += TimeStamps[i]; 
	}
	UE_LOG(LogTemp, Warning, TEXT("Total Time: %f"), TotalTime);

	TimeStamps.Empty();

	if(LevelReference)
	{
		UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(),LevelReference); // loads the next level
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No Next Level has been set!"));
	}
}
