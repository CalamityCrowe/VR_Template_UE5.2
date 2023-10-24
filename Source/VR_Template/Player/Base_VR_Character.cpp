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

	m_VROrigin = CreateOptionalDefaultSubobject<USceneComponent>(TEXT("VR Origin"));
	m_VROrigin->SetupAttachment(GetCapsuleComponent());

	m_Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	m_Camera->SetupAttachment(m_VROrigin);


	m_LeftController = CreateOptionalDefaultSubobject<UMotionControllerComponent>(TEXT("Left Motion Controller"));
	m_RightController = CreateOptionalDefaultSubobject<UMotionControllerComponent>(TEXT("Right Motion Controller"));

	m_LeftController->MotionSource = FName(TEXT("Left"));
	m_RightController->MotionSource = FName(TEXT("Right"));

	m_LeftController->SetupAttachment(m_VROrigin);
	m_RightController->SetupAttachment(m_VROrigin);

	auto MeshAsset = ConstructorHelpers::FObjectFinder<USkeletalMesh>(TEXT("SkeletalMesh'/Game/Characters/MannequinsXR/Meshes/SKM_MannyXR_left.SKM_MannyXR_left'"));

	m_HandLeft = CreateOptionalDefaultSubobject<UMannequin_Hands>(TEXT("Left Hand Mesh"));
	m_HandLeft->SetupAttachment(m_LeftController);
	m_HandLeft->SetSkeletalMesh(MeshAsset.Object);
	m_HandLeft->SetRelativeLocation(FVector(-2.98126, -3.5, 4.561753));
	m_HandLeft->SetRelativeRotation(FRotator(-25, -179.999908, 89.99998));
	m_HandLeft->mb_isMirrored = true;


	MeshAsset = ConstructorHelpers::FObjectFinder<USkeletalMesh>(TEXT("SkeletalMesh'/Game/Characters/MannequinsXR/Meshes/SKM_MannyXR_right.SKM_MannyXR_right'"));
	m_HandRight = CreateOptionalDefaultSubobject<UMannequin_Hands>(TEXT("Right Hand Mesh"));
	m_HandRight->SetupAttachment(m_RightController);
	m_HandRight->SetSkeletalMesh(MeshAsset.Object);
	m_HandRight->SetRelativeLocation(FVector(-2.98126, 3.5, 4.561753));
	m_HandRight->SetRelativeRotation(FRotator(-25, 0, 89.999999));
	m_HandRight->mb_isMirrored = false;


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
	AllignColliderToHMD(); // used for alligning the headset to the collision when moving
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
				InputSystem->AddMappingContext(m_InputMappingContext, 0); // assigns the mapping contexts as the highest priority
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


		PEI->BindAction(m_InputActions->m_InputLeftAnalog, ETriggerEvent::Triggered, this, &ABase_VR_Character::MovePlayer); // binds the movement to the player 
		PEI->BindAction(m_InputActions->m_InputRightAnalog, ETriggerEvent::Triggered, this, &ABase_VR_Character::TurnPlayer); // binds the rotation to the player 

#pragma region Grab Components
		PEI->BindAction(m_InputActions->m_InputLeftGrip, ETriggerEvent::Started, this, &ABase_VR_Character::GrabObjectLeft); 
		PEI->BindAction(m_InputActions->m_InputLeftGrip, ETriggerEvent::Completed, this, &ABase_VR_Character::ReleaseObjectLeft);
		PEI->BindAction(m_InputActions->m_InputRightGrip, ETriggerEvent::Started, this, &ABase_VR_Character::GrabObjectRight);
		PEI->BindAction(m_InputActions->m_InputRightGrip, ETriggerEvent::Completed, this, &ABase_VR_Character::ReleaseObjectRight);
#pragma endregion

	}

}

void ABase_VR_Character::AllignColliderToHMD()
{
	FVector camLoc = FVector(m_Camera->GetComponentLocation().X, m_Camera->GetComponentLocation().Y, GetActorLocation().Z); // grabs the camera location and gets the actors Z location
	FVector actLoc = GetActorLocation(); // grabs the actors location
	if (abs(camLoc.SquaredLength() - actLoc.SquaredLength()) > 1000) // checks if the absolute distance is greater than 1000
	{
		FVector newOffset = camLoc - actLoc; // gets the difference for the newOffset by subtracting the actor location from the camera location
		GetCapsuleComponent()->AddWorldOffset(newOffset); // adds the new offset to the capsule component

		newOffset = UKismetMathLibrary::NegateVector(newOffset); // negates the offset vector

		m_VROrigin->AddWorldOffset(newOffset); //applies the offset to the VR Origin
	}
}

void ABase_VR_Character::MovePlayer(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>(); // gets the axis value from the input value

#if WITH_EDITOR
	GEngine->AddOnScreenDebugMessage(0, 0.1f, FColor::Yellow, FString::Printf(TEXT("CONTROLLER: %f , %f"), MovementVector.X, MovementVector.Y));
#endif
	
	// applies the movement to the characters movement components based on the forward vector
	AddMovementInput(m_Camera->GetForwardVector(), MovementVector.Y);
	AddMovementInput(m_Camera->GetRightVector(), MovementVector.X);

#if WITH_EDITOR
	GEngine->AddOnScreenDebugMessage(0, 0.1f, FColor::Yellow, FString::Printf(TEXT("CONTROLLER NULL")));
#endif
}

void ABase_VR_Character::TurnPlayer(const FInputActionValue& Value)
{
	if (Controller != nullptr) // checks if the controller is valid
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
	if (UVR_GrabComponent* nearestComp = GetGrabComponentNearController(m_LeftController)) // checks if the grab attempt near the controller is successful for grabbing stuff 
	{
		if (nearestComp->TryGrab(m_LeftController)) // checks if the try is successful with the left controller
		{
			m_HeldLeft = nearestComp; // assigns the nearest component to the heldLeft 
			if (&m_HeldLeft == &m_HeldRight) { m_HeldRight == nullptr; } // checks if the held left and right are the same and if so held right is set to null
		}
	}
}

void ABase_VR_Character::GrabObjectRight(const FInputActionValue& Value)
{
	if (UVR_GrabComponent* nearestComp = GetGrabComponentNearController(m_RightController)) // checks if the grab attempt near the controller is successful for grabbing stuff 
	{
		if (nearestComp->TryGrab(m_RightController))// checks if the try is successful with the right controller
		{
			m_HeldRight = nearestComp; // assigns the nearest component to the held right
			if (&m_HeldRight == &m_HeldLeft) { m_HeldLeft == nullptr; } // checks if the held left and right are the same and if so held left is set to null
		}
	}
}

void ABase_VR_Character::ReleaseObjectLeft(const FInputActionValue& Value)
{
	if (IsValid(m_HeldLeft)) // checks if the held left has a valid value
	{
		if (m_HeldLeft->TryRelease()) // checks if try release is true 
		{
			m_HeldLeft = nullptr; // sets the held left to nullptr
#if WITH_EDITOR
			GEngine->AddOnScreenDebugMessage(100, 20, FColor::Purple, TEXT("Left Release"));
#endif
		}
	}
}

void ABase_VR_Character::ReleaseObjectRight(const FInputActionValue& Value)
{
	if (IsValid(m_HeldRight))// checks if the held right has a valid value
	{
		if (m_HeldRight->TryRelease()) // checks if try release is true 
		{
			m_HeldRight = nullptr; // sets the held right to nullptr
#if WITH_EDITOR
			GEngine->AddOnScreenDebugMessage(100, 20, FColor::Purple, TEXT("Right Release"));
#endif
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
	const TArray<AActor*> ignoreActor{ this };

	bool bHasHit = UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), LocalGripPos, LocalGripPos, m_GrabRadius, traceObjects, false, ignoreActor, EDrawDebugTrace::Persistent, hitResult, true); 

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
				if (sqLength <= m_LocalNearestDistance) // checks if the squared length is less or equal to the local nearest distance
				{
					m_LocalNearestDistance = sqLength; // sets the local nearest distance to the squared length
					LocalGrabComponent = GrabPoints[i]; // sets the local grab point to the current grab point
				}
			}
		}
	}

	return LocalGrabComponent;
}

