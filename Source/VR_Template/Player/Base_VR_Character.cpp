// Fill out your copyright notice in the Description page of Project Settings.

#pragma region Includes
#include "Base_VR_Character.h"

#include "MotionControllerComponent.h" // this is used in order to get the motion controllers in the game
#include "Camera/CameraComponent.h" // gets the camera for using in the game
#include "GameFramework/CharacterMovementComponent.h" // gets the functionallity of the movement component
#include "Components/CapsuleComponent.h"
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
#include "Kismet/KismetMathLibrary.h"



#pragma endregion

// Sets default values
ABase_VR_Character::ABase_VR_Character()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	VROrigin = CreateOptionalDefaultSubobject<USceneComponent>(TEXT("VR Origin"));
	VROrigin->SetupAttachment(GetCapsuleComponent());

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(VROrigin);


	LeftController = CreateOptionalDefaultSubobject<UMotionControllerComponent>(TEXT("Left Motion Controller"));
	RightController = CreateOptionalDefaultSubobject<UMotionControllerComponent>(TEXT("Right Motion Controller"));

	LeftController->MotionSource = FName(TEXT("Left"));
	RightController->MotionSource = FName(TEXT("Right"));

	LeftController->SetupAttachment(VROrigin); RightController->SetupAttachment(VROrigin);



	HandLeft = CreateOptionalDefaultSubobject<UMannequin_Hands>(TEXT("Left Hand Mesh"));
	HandLeft->SetupAttachment(LeftController);
	HandLeft->SetRelativeLocation(FVector(-2.98126, -3.5, 4.561753));
	HandLeft->SetRelativeRotation(FRotator(-25, -179.999908, 89.99998));
	HandLeft->Mirror = true;


	HandRight = CreateOptionalDefaultSubobject<UMannequin_Hands>(TEXT("Right Hand Mesh"));
	HandRight->SetupAttachment(RightController);
	HandRight->SetRelativeLocation(FVector(-2.98126, 3.5, 4.561753));
	HandRight->SetRelativeRotation(FRotator(-25, 0, 89.999999));
	HandRight->Mirror = false;


}

// Called when the game starts or when spawned
void ABase_VR_Character::BeginPlay()
{
	Super::BeginPlay();
#if PLATFORM_WINDOWS

	UE_LOG(LogTemp, Warning, TEXT("Running on Windows"));


#elif PLATFORM_PS5 

	UE_LOG(LogTemp, Warning, TEXT("Running on other device"));

#endif
}

// Called every frame
void ABase_VR_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AllignColliderToHMD(); // used for alligning the headset to the collision when moving




	//UE_LOG(LogTemp, Warning, TEXT("Actor Location: %s"), *GetActorLocation().ToString());
}

// Called to bind functionality to input
void ABase_VR_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
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

		if (UInputConfigData* PlayerActions = Cast<UInputConfigData>(InputActions))
		{
			PEI->BindAction(PlayerActions->InputLeftAnalog_X, ETriggerEvent::Triggered, this, &ABase_VR_Character::HorizontalMovement); // binds the movement to the player 
			PEI->BindAction(PlayerActions->InputLeftAnalog_Y, ETriggerEvent::Triggered, this, &ABase_VR_Character::VerticalMovement); // binds the movement to the player 
			PEI->BindAction(PlayerActions->InputRightAnalog, ETriggerEvent::Triggered, this, &ABase_VR_Character::TurnPlayer); // binds the rotation to the player 
			PEI->BindAction(PlayerActions->InputRightAnalog, ETriggerEvent::Started, this, &ABase_VR_Character::SnapTurn); // binds the rotation to the player 
			PEI->BindAction(PlayerActions->InputRightAnalog_Press, ETriggerEvent::Started, this, &ABase_VR_Character::ToggleSnapTurn); // binds the rotation to the player
#pragma region Grab Components
			PEI->BindAction(PlayerActions->InputLeftGrip, ETriggerEvent::Started, this, &ABase_VR_Character::GrabObjectLeft); // binds the left grip to the player
			PEI->BindAction(PlayerActions->InputLeftGrip, ETriggerEvent::Completed, this, &ABase_VR_Character::ReleaseObjectLeft); // binds the left grip to the player
			PEI->BindAction(PlayerActions->InputRightGrip, ETriggerEvent::Started, this, &ABase_VR_Character::GrabObjectRight); // binds the right grip to the player
			PEI->BindAction(PlayerActions->InputRightGrip, ETriggerEvent::Completed, this, &ABase_VR_Character::ReleaseObjectRight); // binds the right grip to the player


#pragma endregion

		}

	}

}

void ABase_VR_Character::AllignColliderToHMD()
{
	FVector camLoc = FVector(Camera->GetComponentLocation().X, Camera->GetComponentLocation().Y, GetActorLocation().Z); // grabs the camera location and gets the actors Z location
	FVector actLoc = GetActorLocation(); // grabs the actors location
	if (abs(camLoc.SquaredLength() - actLoc.SquaredLength()) > 1000) // checks if the absolute distance is greater than 1000
	{
		FVector newOffset = camLoc - actLoc; // gets the difference for the newOffset by subtracting the actor location from the camera location
		GetCapsuleComponent()->AddWorldOffset(newOffset); // adds the new offset to the capsule component

		newOffset = UKismetMathLibrary::NegateVector(newOffset); // negates the offset vector

		VROrigin->AddWorldOffset(newOffset); //applies the offset to the VR Origin
	}
}

void ABase_VR_Character::VerticalMovement(const FInputActionValue& Value)
{
	if (Controller != nullptr)
	{
		float MovementScale = Value.Get<float>(); // gets the axis value from the input value

		AddMovementInput(Camera->GetForwardVector(), MovementScale * WalkSpeedScale); // applies the movement to the characters movement components based on the up vector
	}
}

void ABase_VR_Character::HorizontalMovement(const FInputActionValue& Value)
{
	if (Controller != nullptr)
	{
		float MovementScale = Value.Get<float>(); // gets the axis value from the input value 

		AddMovementInput(Camera->GetRightVector(), MovementScale * WalkSpeedScale); // applies the movement to the characters movement components based on the right vector
	}
}

void ABase_VR_Character::TurnPlayer(const FInputActionValue& Value)
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

void ABase_VR_Character::SnapTurn(const FInputActionValue& Value)
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



void ABase_VR_Character::GrabObjectLeft(const FInputActionValue& Value)
{
	if (UVR_GrabComponent* nearestComp = GetGrabComponentNearController(LeftController)) // checks if the grab attempt near the controller is successful for grabbing stuff 
	{
		if (nearestComp->TryGrab(LeftController)) // checks if the try is successful with the left controller
		{
			HeldLeft = nearestComp; // assigns the nearest component to the heldLeft 
			if (HeldLeft == HeldRight) { HeldRight = nullptr; } // checks if the held left and right are the same and if so held right is set to null
		}
	}
}

void ABase_VR_Character::GrabObjectRight(const FInputActionValue& Value)
{
	if (UVR_GrabComponent* nearestComp = GetGrabComponentNearController(RightController)) // checks if the grab attempt near the controller is successful for grabbing stuff 
	{
		if (nearestComp->TryGrab(RightController))// checks if the try is successful with the right controller
		{
			HeldRight = nearestComp; // assigns the nearest component to the held right
			if (HeldRight == HeldLeft) { HeldLeft = nullptr; } // checks if the held left and right are the same and if so held left is set to null
		}
	}
}

void ABase_VR_Character::ReleaseObjectLeft(const FInputActionValue& Value)
{
	if (HeldLeft != nullptr) // checks if the held left has a valid value
	{
		if (HeldLeft->TryRelease()) // checks if try release is true 
		{
			HeldLeft = nullptr; // sets the held left to nullptr
		}
	}
}

void ABase_VR_Character::ReleaseObjectRight(const FInputActionValue& Value)
{
	if (HeldRight != nullptr)// checks if the held right has a valid value
	{
		if (HeldRight->TryRelease()) // checks if try release is true 
		{
			HeldRight = nullptr; // sets the held right to nullptr
		}
	}
}


UVR_GrabComponent* ABase_VR_Character::GetGrabComponentNearController(UMotionControllerComponent* controllerReference)
{
	UVR_GrabComponent* LocalGrabComponent = nullptr; // creates a local grab component and sets it to null

	FHitResult hitResult; // creates a hit result

	FVector LocalGripPos = controllerReference->GetComponentLocation();

	TArray< TEnumAsByte<EObjectTypeQuery>> traceObjects;
	traceObjects.Add(UEngineTypes::ConvertToObjectType(ECC_PhysicsBody));
	const TArray<AActor*> ignoreActor{ this };


	bool bHasHit = UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), LocalGripPos, LocalGripPos, GrabRadius, traceObjects, false, ignoreActor, EDrawDebugTrace::Persistent, hitResult, true);

	if (bHasHit) // checks if the sphere trace has hit something
	{
		TArray<UVR_GrabComponent*> GrabPoints;
		hitResult.GetActor()->GetComponents(GrabPoints); // gets the actor from the hit results
		if (GrabPoints.Num() > 0) // checks if it is greater than 0
		{
			for (int i = 0; i < GrabPoints.Num(); i++) // loops through all the grab points and assigns the nearest one to the local component grabbed
			{
				FVector componentWorldLocation = GrabPoints[i]->GetComponentLocation(); // gets the current component location
				componentWorldLocation -= LocalGripPos; // subtracts the local grip position from the component location
				float sqLength = componentWorldLocation.SquaredLength();
				if (sqLength <= LocalNearestDistance) // checks if the squared length is less or equal to the local nearest distance
				{
					LocalNearestDistance = sqLength; // sets the local nearest distance to the squared length
					LocalGrabComponent = GrabPoints[i]; // sets the local grab point to the current grab point
				}
			}
		}
	}

	return LocalGrabComponent;
}

