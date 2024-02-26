// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomVRLevelScript.h"

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
