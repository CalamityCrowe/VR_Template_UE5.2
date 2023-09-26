// Fill out your copyright notice in the Description page of Project Settings.


#include "VR_GrabComponent.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/KismetMathLibrary.h"
#include "MotionControllerComponent.h" // this is used in order to get the motion controllers in the game
#include "Kismet/GameplayStatics.h" // used for getting the players controllers


// Sets default values for this component's properties
UVR_GrabComponent::UVR_GrabComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UVR_GrabComponent::BeginPlay()
{
	Super::BeginPlay();
	SetShouldSimulateDrop();
	if (UPrimitiveComponent* PrimComponent = Cast<UPrimitiveComponent>(GetAttachParent()))
	{
		PrimComponent->SetCollisionProfileName(FName(TEXT("PhysicsActor")), true); // sets up the collison profiles and tells it to update the overlaps of the object
	}
	// ...

}


// Called every frame
void UVR_GrabComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UVR_GrabComponent::TryGrab(UMotionControllerComponent* MotionController)
{
	switch (m_GrabType)
	{
	case GrabType::None:
		break;
	case GrabType::Free:
	{
		SetPrimativeCompPhysics(false);
		AttachParentToController(MotionController);
		isHeld = true;
		break;

	}
	case GrabType::Snap:
	{		
		SetPrimativeCompPhysics(false);
		AttachParentToController(MotionController);
		isHeld = true;

		FRotator invertedRotation = UKismetMathLibrary::NegateRotator(GetRelativeRotation());

		SetRelativeRotation(invertedRotation, false, nullptr, ETeleportType::TeleportPhysics);

		FVector ParentLocation = GetAttachParent()->GetComponentLocation(); // gets the location of the parent

		FVector newLocation = (GetComponentLocation() - ParentLocation) * -1;

		newLocation = MotionController->GetComponentLocation() + newLocation; // sets the new location based on the motion controllers location + an offset

		SetWorldLocation(newLocation, false, nullptr, ETeleportType::TeleportPhysics); // sets the world location of the object to the new location


	break;
	}
	case GrabType::Custom:
	{
		isHeld = true;
		break;
	}
	}

	if (isHeld)
	{
		return false; // basically says that the object is not held by the user
	}
	MotionControllerReference = MotionController; // assigns a reference to the motion controller

	// the on grabbed delegate needs called here

	if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0)) 
	{
		PC->PlayHapticEffect(OnGrabHapticFeedback, GetHeldByHand(), 1, false);  // plays the haptic feedback effect on the controller

	}

	return true;
}

void UVR_GrabComponent::TryRelease()
{
}

void UVR_GrabComponent::SetShouldSimulateDrop()
{
}

void UVR_GrabComponent::SetPrimativeCompPhysics(bool isSimulated)
{
	if (UPrimitiveComponent* PrimComponent = Cast<UPrimitiveComponent>(GetAttachParent()))
	{
		PrimComponent->SetSimulatePhysics(isSimulated);  // sets the physics of the component
	}
}

EControllerHand UVR_GrabComponent::GetHeldByHand()
{
	if (MotionControllerReference->MotionSource == FName(TEXT("Left"))) // checks if it is the left hand that has grabbed the object
	{
		return EControllerHand::Left;  // returns the value of the left hand 
	}
	else 
	{
		return EControllerHand::Right; // returns the value of the right hand
	}
}

void UVR_GrabComponent::AttachParentToController(UMotionControllerComponent*)
{
}

