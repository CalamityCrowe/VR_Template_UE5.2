// Fill out your copyright notice in the Description page of Project Settings.

#pragma region Includes
#include "Base_VR_Character.h"

#include "MotionControllerComponent.h" // this is used in order to get the motion controllers in the game
#include "Camera/CameraComponent.h" // gets the camera for using in the game
#include "GameFramework/CharacterMovementComponent.h" // gets the functionallity of the movement component

#include "Inputs/InputConfigData.h"
 //these four are used for the files relevant to the Enhanced input system
 //UProject and build.cs need to be edited so it knows about these files
 //UPROJECT needs to know about the plugin to know what one to enable
 //build.cs needs to know about the dependencies so it can link them
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"

#include "VR_Template/VR_Components/Mannequin_Hands.h"
#include "VR_Template/VR_Components/VR_GrabComponent.h"

#include "Kismet/KismetSystemLibrary.h"

#pragma endregion

// Sets default values
ABase_VR_Character::ABase_VR_Character()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	m_LeftController = CreateOptionalDefaultSubobject<UMotionControllerComponent>(TEXT("Left Motion Controller"));
	m_RightController = CreateOptionalDefaultSubobject<UMotionControllerComponent>(TEXT("Right Motion Controller"));

	m_LeftController->MotionSource = FName(TEXT("Left"));
	m_RightController->MotionSource = FName(TEXT("Right"));

	m_HandLeft = CreateOptionalDefaultSubobject<UMannequin_Hands>(TEXT("Left Hand Mesh"));
	m_HandLeft->SetupAttachment(m_LeftController);
	m_HandLeft->mb_isMirrored = true;

	m_HandRight = CreateOptionalDefaultSubobject<UMannequin_Hands>(TEXT("Right Hand Mesh"));
	m_HandRight->SetupAttachment(m_RightController);
	m_HandRight->mb_isMirrored = false;

	GetMesh()->DestroyComponent();

}

// Called when the game starts or when spawned
void ABase_VR_Character::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ABase_VR_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABase_VR_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (APlayerController* PC = Cast<APlayerController>(GetController())) // tries to grab an instance of the player controller
	{
		if (ULocalPlayer* LocalPlayer = PC->GetLocalPlayer())
		{
			if (UEnhancedInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
			{
				InputSystem->ClearAllMappings(); // clears any mapping contexts if they had been assigned by mistake
				InputSystem->AddMappingContext(m_InputMappingContext, 0); // assigns the mapping contexts as the highest priority
			}
		}
	}

	if (UEnhancedInputComponent* PEI = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
#pragma region Setup Actions Comments
		/* Bind Action Setup

		 {
			 BindAction(UInputAction,
			 ETriggerEvent,
			 CharacterObject(this),
			 Function Reference (&Template::Do Thing)
			)
		 }

		 ETriggerEvents
		 {
			 None         = (0x0),
			 Triggered    = (1 << 0), Triggering occurred after one or more processing ticks.
			 Started      = (1 << 1), An event has occurred that has begun Trigger evaluation. Note: Triggered may also occur this frame, but this event will always be fired first.
			 Ongoing      = (1 << 2), Triggering is still being processed.
			 Canceled     = (1 << 3), Triggering has been canceled.
			 Completed    = (1 << 4), The trigger state has transitioned from Triggered to None this frame, i.e. Triggering has finished.
		 }
		*/
#pragma endregion


		PEI->BindAction(m_InputActions->m_InputLeftAnalog, ETriggerEvent::Triggered, this, &ABase_VR_Character::MovePlayer); // binds an action to the input component
		PEI->BindAction(m_InputActions->m_InputRightAnalog, ETriggerEvent::Triggered, this, &ABase_VR_Character::TurnPlayer); // binds an action to the input component

		PEI->BindAction(m_InputActions->m_InputLeftGrip, ETriggerEvent::Triggered, this, &ABase_VR_Character::GrabObjectLeft);
		PEI->BindAction(m_InputActions->m_InputLeftGrip, ETriggerEvent::Canceled, this, &ABase_VR_Character::ReleaseObjectLeft);
		PEI->BindAction(m_InputActions->m_InputRightGrip, ETriggerEvent::Triggered, this, &ABase_VR_Character::GrabObjectRight);
		PEI->BindAction(m_InputActions->m_InputRightGrip, ETriggerEvent::Canceled, this, &ABase_VR_Character::ReleaseObjectRight);
	}

}

void ABase_VR_Character::MovePlayer(const FInputActionValue& Value)
{
	if (Controller) // checks that the controller is actually valid before it is used
	{
		const FVector2D MoveValue = Value.Get<FVector2D>(); // this gets the axis values for the input 
		const FRotator MovementRotation = FRotator(0, Controller->GetControlRotation().Yaw, 0);

		if (MoveValue.Y != 0) // checks that the controller has in put in the Y axis (forward or backwards)
		{
			FVector ForwardDir = MovementRotation.RotateVector(FVector::ForwardVector); // gets the forward vector so it knows the  direction it should move the player in
			AddMovementInput(ForwardDir, MoveValue.Y);  // applies the movement to the player
		}

		if (MoveValue.X != 0) // checks that the controller has an input on the X axis of the input (left or right)
		{
			FVector RightDir = MovementRotation.RotateVector(FVector::RightVector);  // gets the right vector so it knows the directions it should move the player
			AddMovementInput(RightDir, MoveValue.X);  // Applies the movement to the player
		}

	}
}

void ABase_VR_Character::TurnPlayer(const FInputActionValue& Value)
{
	if (Controller) // checks if the controller is valid
	{
		const FVector2D TurnRate = Value.Get<FVector2D>(); // gets the vector 2d from the input action 

		if (TurnRate.Y != 0) // checks if the y axis is not 0, this means that the mouse or whatever the input is has moved slightly for it to register a vertical input
		{
			AddControllerPitchInput(TurnRate.Y); // applies the vertical input of the mouse into the controller pitch input
		}
		if (TurnRate.X != 0)  // checks if the x axis is not 0, this means that the mouse or whatever the input is has moved slightly for it to register a horizontal input
		{
			AddControllerYawInput(TurnRate.X); // applies the horizontal input of the mouse into the controller yaw input
		}
	}
}

void ABase_VR_Character::GrabObjectLeft(const FInputActionValue& Value)
{
	if (UVR_GrabComponent* nearestComp = GetGrabComponentNearController(m_LeftController))
	{
		if (nearestComp->TryGrab(m_LeftController))
		{
			m_HeldLeft = nearestComp;
			if (m_HeldLeft == m_HeldRight) { m_HeldRight == nullptr; }
		}
	}
}

void ABase_VR_Character::GrabObjectRight(const FInputActionValue& Value)
{
	if (UVR_GrabComponent* nearestComp = GetGrabComponentNearController(m_RightController))
	{
		if (nearestComp->TryGrab(m_RightController))
		{
			m_HeldRight = nearestComp;
			if (m_HeldRight == m_HeldLeft) { m_HeldLeft == nullptr; }
		}
	}
}

void ABase_VR_Character::ReleaseObjectLeft(const FInputActionValue& Value)
{
	if (m_HeldLeft) 
	{
		if (m_HeldLeft->TryRelease()) 
		{
			m_HeldLeft = nullptr; 
		}
	}
}

void ABase_VR_Character::ReleaseObjectRight(const FInputActionValue& Value)
{
	if (m_HeldRight)
	{
		if (m_HeldRight->TryRelease())
		{
			m_HeldRight = nullptr;
		}
	}
}

UVR_GrabComponent* ABase_VR_Character::GetGrabComponentNearController(UMotionControllerComponent* controllerReference)
{
	UVR_GrabComponent* LocalGrabComponent = nullptr;

	FHitResult hitResult;

	FVector LocalGripPos = controllerReference->GetComponentLocation();

	TArray< TEnumAsByte<EObjectTypeQuery>> traceObjects;
	traceObjects.Add(UEngineTypes::ConvertToObjectType(ECC_PhysicsBody));
	const TArray<AActor*> ignoreActor;
	bool bHasHit = UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), LocalGripPos, LocalGripPos, m_GrabRadius, traceObjects, false, ignoreActor, EDrawDebugTrace::None, hitResult, true);

	if (bHasHit)
	{
		TArray<UVR_GrabComponent*> GrabPoints;
		hitResult.GetActor()->GetComponents(GrabPoints);
		if (GrabPoints.Num() > 0)
		{
			for (int i = 0; i < GrabPoints.Num(); i++)
			{
				FVector componentWorldLocation = GrabPoints[i]->GetComponentLocation();
				componentWorldLocation -= LocalGripPos;
				float sqLength = componentWorldLocation.SquaredLength();
				if (sqLength <= m_LocalNearestDistance)
				{
					m_LocalNearestDistance = sqLength;
					LocalGrabComponent = GrabPoints[i];
				}
			}
		}
	}

	return LocalGrabComponent;
}

