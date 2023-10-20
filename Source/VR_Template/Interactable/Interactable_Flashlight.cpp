// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable_Flashlight.h"
#include "Components/SpotLightComponent.h"

AInteractable_Flashlight::AInteractable_Flashlight()
{
	m_Light = CreateOptionalDefaultSubobject<USpotLightComponent>(TEXT("Spot light"));
	m_Light->SetupAttachment(GetMesh());

	m_Light->OuterConeAngle = 20.0f;
	m_Light->InnerConeAngle = 10.0f;
	m_Light->Intensity = 10000.f;
	m_Light->LightColor = FColor(255, 244, 229, 255);
	m_Light->CastShadows = true;
	m_Light->bAffectsWorld = true;


}

void AInteractable_Flashlight::BeginPlay()
{
	Super::BeginPlay();

	GEngine->AddOnScreenDebugMessage(23, 5, FColor::Red, TEXT("Nipples"));
}

void AInteractable_Flashlight::Tick(float Delta)
{
	Super::Tick(Delta);

}

void AInteractable_Flashlight::ToggleFlashlight()
{
	if (m_Light->IsActive()) 
	{
		m_Light->Deactivate(); 
	}
	else
	{
		m_Light->Activate(true); 
	}
}
