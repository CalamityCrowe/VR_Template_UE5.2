// Fill out your copyright notice in the Description page of Project Settings.


#include "VR_Menu_Character.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Inputs/MenuInputData.h"
#include "Inputs/InputConfigData.h"

#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Components/WidgetInteractionComponent.h"
#include "VR_Template/VR_Components/Mannequin_Hands.h"
#include "HeadMountedDisplayFunctionLibrary.h"

#include "NiagaraComponent.h"


AVR_Menu_Character::AVR_Menu_Character() : ABase_VR_Character()
{
	LeftWidgetInteractionComponent = CreateOptionalDefaultSubobject<UWidgetInteractionComponent>(TEXT("Left Interaction"));
	LeftWidgetInteractionComponent->SetupAttachment(GetLeftHandMesh());

	RightWidgetInteractionComponent = CreateOptionalDefaultSubobject<UWidgetInteractionComponent>(TEXT("Right Interaction"));
	RightWidgetInteractionComponent->SetupAttachment(GetRightHandMesh());

	LeftHandParticle = CreateOptionalDefaultSubobject<UNiagaraComponent>(TEXT("Laser"));
	LeftHandParticle->SetupAttachment(LeftWidgetInteractionComponent);

	RightHandParticle = CreateOptionalDefaultSubobject<UNiagaraComponent>(TEXT("Raser"));
	RightHandParticle->SetupAttachment(RightWidgetInteractionComponent);
	Timer = 0;
}

AVR_Menu_Character::~AVR_Menu_Character()
{

}

void AVR_Menu_Character::BeginPlay()
{
	Super::BeginPlay();
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition(0, EOrientPositionSelector::Orientation); // resets the orientation and position of the player

}

void AVR_Menu_Character::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	HandleLeftWidget();
	HandleRightWidget();
	AllignColliderToHMD(); // used for alligning the headset to the collision when moving

	Timer += DeltaSeconds / 4;
	if (Timer >= 1.f)
	{
		Timer = 0.0f;
	}

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
				InputSystem->AddMappingContext(InputMappingContext, 1); // assigns the movement context as the highest priority
				InputSystem->AddMappingContext(MovementContext, 0); // assigns the movement context as the second highest priority
			}
		}
	}

	if (UEnhancedInputComponent* PEI = Cast<UEnhancedInputComponent>(PlayerInputComponent)) // attempts to cast to the enhanced input system
	{

		if (UMenuInputData* MenuActions = Cast<UMenuInputData>(InputActions))
		{
			PEI->BindAction(MenuActions->IA_RightTrigger, ETriggerEvent::Started, this, &AVR_Menu_Character::RightTriggerPressed);
			PEI->BindAction(MenuActions->IA_RightTrigger, ETriggerEvent::Completed, this, &AVR_Menu_Character::RightTriggerReleased);
			PEI->BindAction(MenuActions->IA_LeftTrigger, ETriggerEvent::Started, this, &AVR_Menu_Character::LeftTriggerPressed);
			PEI->BindAction(MenuActions->IA_LeftTrigger, ETriggerEvent::Completed, this, &AVR_Menu_Character::LeftTriggerReleased);
		}
		if (MovementConfig)
		{
			PEI->BindAction(MovementConfig->InputLeftAnalog_X, ETriggerEvent::Triggered, this, &AVR_Menu_Character::HorizontalMovement); // binds the movement to the player 
			PEI->BindAction(MovementConfig->InputLeftAnalog_Y, ETriggerEvent::Triggered, this, &AVR_Menu_Character::VerticalMovement); // binds the movement to the player 
			PEI->BindAction(MovementConfig->InputRightAnalog, ETriggerEvent::Triggered, this, &AVR_Menu_Character::TurnPlayer); // binds the rotation to the player 
			PEI->BindAction(MovementConfig->InputRightAnalog, ETriggerEvent::Started, this, &AVR_Menu_Character::SnapTurn); // binds the rotation to the player 
			PEI->BindAction(MovementConfig->InputRightAnalog_Press, ETriggerEvent::Started, this, &AVR_Menu_Character::ToggleSnapTurn); // binds the rotation to the player		}
		}
	}
}

void AVR_Menu_Character::VerticalMovement(const FInputActionValue& Value)
{
	ABase_VR_Character::VerticalMovement(Value);
}
void AVR_Menu_Character::HorizontalMovement(const FInputActionValue& Value)
{
	ABase_VR_Character::HorizontalMovement(Value);
}
void AVR_Menu_Character::TurnPlayer(const FInputActionValue& Value)
{
	ABase_VR_Character::TurnPlayer(Value);
}
void AVR_Menu_Character::SnapTurn(const FInputActionValue& Value)
{
	ABase_VR_Character::SnapTurn(Value);
}

void AVR_Menu_Character::ToggleSnapTurn(const FInputActionValue& Value)
{
	ABase_VR_Character::ToggleSnapTurn(Value);
}

void AVR_Menu_Character::RightTriggerPressed()
{
	if (RightWidgetInteractionComponent) // checks to see if the right widget interaction component is valid
	{
		RightWidgetInteractionComponent->PressPointerKey(EKeys::LeftMouseButton); // presses the left mouse button
	}
}

void AVR_Menu_Character::LeftTriggerPressed()
{
	if (LeftWidgetInteractionComponent) // checks to see if the left widget interaction component is valid
	{
		LeftWidgetInteractionComponent->PressPointerKey(EKeys::LeftMouseButton); // presses the left mouse button
	}

}

void AVR_Menu_Character::LeftTriggerReleased()
{
	if (LeftWidgetInteractionComponent) // checks to see if the left widget interaction component is valid
	{
		LeftWidgetInteractionComponent->ReleasePointerKey(EKeys::LeftMouseButton); // releases the left mouse button
	}
}

void AVR_Menu_Character::RightTriggerReleased()
{
	if (RightWidgetInteractionComponent)
	{
		RightWidgetInteractionComponent->ReleasePointerKey(EKeys::LeftMouseButton); // releases the left mouse button
	}
}

void AVR_Menu_Character::HandleRightWidget()
{
	if (RightWidgetInteractionComponent->IsOverHitTestVisibleWidget()) // checks to see if the right widget interaction component is over a visible widget
	{
		FHitResult HitResult = RightWidgetInteractionComponent->GetLastHitResult(); // gets the last hit result
		RightHandParticle->SetNiagaraVariableVec3("EndPoint", HitResult.ImpactPoint); // sets the end point of the particle to the impact point
	}
	else
	{
		FVector HandLocation = RightWidgetInteractionComponent->GetComponentLocation(); // gets the location of the player
		FVector HandForward = RightWidgetInteractionComponent->GetForwardVector() * RightWidgetInteractionComponent->InteractionDistance; // gets the forward vector of the player
		RightHandParticle->SetNiagaraVariableVec3("EndPoint", HandLocation + HandForward); // sets the end point of the particle to the player
	}
	RightHandParticle->SetNiagaraVariableFloat("Time", Timer);
}

void AVR_Menu_Character::HandleLeftWidget()
{
	if (LeftWidgetInteractionComponent->IsOverHitTestVisibleWidget()) // checks to see if the left widget interaction component is over a visible widget	
	{
		FHitResult HitResult = LeftWidgetInteractionComponent->GetLastHitResult(); // gets the last hit result 
		LeftHandParticle->SetNiagaraVariableVec3("EndPoint", HitResult.ImpactPoint); // sets the end point of the particle to the impact point
	}
	else
	{
		FVector HandLocation = LeftWidgetInteractionComponent->GetComponentLocation(); // gets the location of the player
		FVector HandForward = LeftWidgetInteractionComponent->GetForwardVector() * LeftWidgetInteractionComponent->InteractionDistance; // gets the forward vector of the player
		LeftHandParticle->SetNiagaraVariableVec3("EndPoint", HandLocation + HandForward);
	}
	LeftHandParticle->SetNiagaraVariableFloat("Time", Timer);

}
