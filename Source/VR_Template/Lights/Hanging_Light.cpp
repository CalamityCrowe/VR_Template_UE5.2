// Fill out your copyright notice in the Description page of Project Settings.


#include "Hanging_Light.h"
#include "CableComponent.h"
#include "Components/SpotLightComponent.h"


AHanging_Light::AHanging_Light()
{
	SpotLight = CreateOptionalDefaultSubobject<USpotLightComponent>(TEXT("Light Cable"));
	SpotLight->SetupAttachment(GetMesh()); 
}

void AHanging_Light::BeginPlay()
{
	Super::BeginPlay();


}

void AHanging_Light::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}
