// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable_Flashlight.h"
#include "Components/SpotLightComponent.h"

AInteractable_Flashlight::AInteractable_Flashlight()
{
	m_Light = CreateOptionalDefaultSubobject<USpotLightComponent>(TEXT("Spot light")); 
	m_Light->SetupAttachment(GetMesh()); 
}
