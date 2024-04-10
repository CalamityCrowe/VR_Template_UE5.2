// Fill out your copyright notice in the Description page of Project Settings.


#include "VR_Menu_Character.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Inputs/MenuInputData.h"
#include "Components/InputComponent.h"
#include "Components/WidgetInteractionComponent.h"
#include "NiagaraComponent.h"
#include "VR_Template/VR_Components/Mannequin_Hands.h"




AVR_Menu_Character::AVR_Menu_Character()
{
	LeftWidgetInteractionComponent = CreateOptionalDefaultSubobject<UWidgetInteractionComponent>(TEXT("Left Interaction"));
	LeftWidgetInteractionComponent->SetupAttachment(GetLeftHandMesh());

	RightWidgetInteractionComponent = CreateOptionalDefaultSubobject<UWidgetInteractionComponent>(TEXT("Right Interaction"));
	RightWidgetInteractionComponent->SetupAttachment(GetRightHandMesh());

	LeftHandParticle = CreateOptionalDefaultSubobject<UNiagaraComponent>(TEXT("Laser"));
	LeftHandParticle->SetupAttachment(LeftWidgetInteractionComponent);

	RightHandParticle = CreateOptionalDefaultSubobject<UNiagaraComponent>(TEXT("Raser"));
	RightHandParticle->SetupAttachment(RightWidgetInteractionComponent);

}

void AVR_Menu_Character::BeginPlay()
{
	Super::BeginPlay();

}

void AVR_Menu_Character::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	HandleLeftWidget();
	HandleRightWidget();
}

void AVR_Menu_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (APlayerController* PC = Cast<APlayerController>(GetController())) // tries to grab an instance of the player controller
	{
		if (ULocalPlayer* LocalPlayer = PC->GetLocalPlayer()) // gets the local player from the controller
		{
			if (UEnhancedInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())  // gets the enhanced input from the local input system
			{
				InputSystem->ClearAllMappings(); // clears any mapping contexts if they had been assigned by mistake
				InputSystem->AddMappingContext(InputMappingContext, 0); // assigns the mapping contexts as the highest priority
			}
		}
	}

	if (UEnhancedInputComponent* PEI = Cast<UEnhancedInputComponent>(PlayerInputComponent)) // attempts to cast to the enhanced input system
	{
		if (UMenuInputData* MenuActions = Cast<UMenuInputData>(InputActions))
		{
			PEI->BindAction(MenuActions->IA_RightTrigger, ETriggerEvent::Started, this, &AVR_Menu_Character::RightTriggerPressed);
			PEI->BindAction(MenuActions->IA_LeftTrigger, ETriggerEvent::Started, this, &AVR_Menu_Character::LeftTriggerPressed);
		}
	}
}

void AVR_Menu_Character::RightTriggerPressed()
{
	if (RightWidgetInteractionComponent)
	{
		RightWidgetInteractionComponent->PressPointerKey(EKeys::LeftMouseButton);
	}
}

void AVR_Menu_Character::LeftTriggerPressed()
{
	if (LeftWidgetInteractionComponent)
	{
		LeftWidgetInteractionComponent->PressPointerKey(EKeys::LeftMouseButton);
	}

}

void AVR_Menu_Character::HandleRightWidget()
{
	if (RightWidgetInteractionComponent->IsOverHitTestVisibleWidget())
	{
		FHitResult HitResult = RightWidgetInteractionComponent->GetLastHitResult();
		if (RightHandParticle->IsActive() == false)
		{
			RightHandParticle->Activate();
		}
		RightHandParticle->SetNiagaraVariableVec3("EndPoint", HitResult.ImpactPoint);
	}
	else
	{
		if (RightHandParticle->IsActive())
		{
			RightHandParticle->Deactivate();
		}
	}
}

void AVR_Menu_Character::HandleLeftWidget()
{
	if (LeftWidgetInteractionComponent->IsOverHitTestVisibleWidget())
	{
		FHitResult HitResult = LeftWidgetInteractionComponent->GetLastHitResult();
		if (LeftHandParticle->IsActive() == false)
		{
			LeftHandParticle->Activate();
		}
		LeftHandParticle->SetNiagaraVariableVec3("EndPoint", HitResult.ImpactPoint);
	}
	else
	{
		if (LeftHandParticle->IsActive())
		{
			LeftHandParticle->Deactivate();
		}
	}

}
