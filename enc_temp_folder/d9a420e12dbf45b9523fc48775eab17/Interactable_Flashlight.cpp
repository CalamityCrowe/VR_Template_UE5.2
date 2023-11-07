// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable_Flashlight.h"
#include "Components/SpotLightComponent.h"
#include "Kismet/GameplayStatics.h"

#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"

#include "VR_Template/Player/Inputs/InputConfigData.h"
#include "VR_Template/VR_Components/VR_GrabComponent.h"



AInteractable_Flashlight::AInteractable_Flashlight()
{
	SpotLight = CreateOptionalDefaultSubobject<USpotLightComponent>(TEXT("Spot light"));
	SpotLight->SetupAttachment(GetMesh());
	
	SpotLight->OuterConeAngle = 20.0f;
	SpotLight->InnerConeAngle = 10.0f;
	SpotLight->Intensity = 10000.f;
	SpotLight->LightColor = FColor(255, 244, 229, 255);
	SpotLight->CastShadows = true;
	SpotLight->bAffectsWorld = true;
	SpotLight->SetRelativeRotation(FRotator(90, 0, 0));
	SpotLight->SetRelativeLocation(FVector(0, 0, 7.1f));
	auto LightMaterial = ConstructorHelpers::FObjectFinder<UMaterialInterface>(TEXT("Material'/Game/Materials/m_LightFlicker.m_LightFlicker'"));
	SpotLight->LightFunctionMaterial = LightMaterial.Object; 

	// used for loading in the objects based on their type
	LoadDataTable(EObjectType::Light);

}

void AInteractable_Flashlight::BeginPlay()
{
	ABase_Interactable::BeginPlay();

}

void AInteractable_Flashlight::Tick(float Delta)
{
	Super::Tick(Delta);

}

void AInteractable_Flashlight::ToggleFlashlight()
{
	if (SpotLight->IsActive())
	{
		SpotLight->Deactivate();
	}
	else
	{
		SpotLight->Activate(true);
	}
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		PC->PlayHapticEffect(GetGrabComponent()->GetInteractHapticEffect(), GetGrabComponent()->GetHeldByHand(), 1, false);  // plays the haptic feedback effect on the controller
	}
}

void AInteractable_Flashlight::BindInteractableInput()
{
	ABase_Interactable::BindInteractableInput();

	if (const APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PC->InputComponent))
		{
			if (GetGrabComponent()->GetHeldByHand() == EControllerHand::Left)
			{
				EnhancedInputComponent->BindAction(m_FireActions->InputLeftTrigger, ETriggerEvent::Triggered, this, &AInteractable_Flashlight::ToggleFlashlight);
			}
			else
			{
				EnhancedInputComponent->BindAction(m_FireActions->InputRightTrigger, ETriggerEvent::Triggered, this, &AInteractable_Flashlight::ToggleFlashlight);

			}
		}
	}
}

void AInteractable_Flashlight::UnbindInput()
{
	ABase_Interactable::UnbindInput();
	// this might need additional functionality for unbinding the inputs

}
