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

}

AVR_Menu_Character::~AVR_Menu_Character()
{
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
	AllignColliderToHMD(); // used for alligning the headset to the collision when moving

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
			PEI->BindAction(MenuActions->IA_LeftTrigger, ETriggerEvent::Started, this, &AVR_Menu_Character::LeftTriggerPressed);
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
	if (Controller != nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Vertical Movement"))); // prints to the screen that the vertical movement is being called
		float MovementScale = Value.Get<float>(); // gets the axis value from the input value

		AddMovementInput(GetCamera()->GetForwardVector(), MovementScale * WalkSpeedScale); // applies the movement to the characters movement components based on the up vector
	}
}
void AVR_Menu_Character::HorizontalMovement(const FInputActionValue& Value)
{
	if (Controller != nullptr)
	{
		float MovementScale = Value.Get<float>(); // gets the axis value from the input value 

		AddMovementInput(GetCamera()->GetRightVector(), MovementScale * WalkSpeedScale); // applies the movement to the characters movement components based on the right vector
	}
}
void AVR_Menu_Character::TurnPlayer(const FInputActionValue& Value)
{
	if (Controller != nullptr) // checks if the controller is valid
	{
		const float TurnRate = Value.Get<float>(); // gets the vector 2d from the input action
		if (bSnapTurning == false)
		{
			if (TurnRate != 0)  // checks if the x axis is not 0, this means that the mouse or whatever the input is has moved slightly for it to register a horizontal input
			{
				AddControllerYawInput(TurnRate * TurnScale); // applies the horizontal input of the mouse into the controller yaw input
			}
		}
	}
}
void AVR_Menu_Character::SnapTurn(const FInputActionValue& Value)
{
	if (Controller != nullptr)
	{
		const float TurnRate = Value.Get<float>(); // gets the vector 2d from the input action
		if (bSnapTurning == true)
		{
			if (TurnRate > 0)
				AddControllerYawInput(1 * SnapTurnScale); // applies the horizontal input of the mouse into the controller yaw input
			else
				AddControllerYawInput(-1 * SnapTurnScale); // applies the horizontal input of the mouse into the controller yaw input

		}
	}
}

void AVR_Menu_Character::ToggleSnapTurn(const FInputActionValue& Value)
{
	bSnapTurning = !bSnapTurning;
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
