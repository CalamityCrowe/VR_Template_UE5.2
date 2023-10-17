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


	detachRules = new FDetachmentTransformRules(EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, true);
	detachRules->bCallModify = true;
	detachRules->LocationRule = EDetachmentRule::KeepWorld;
	detachRules->RotationRule = EDetachmentRule::KeepWorld;
	detachRules->ScaleRule = EDetachmentRule::KeepWorld;

	isHeld = false; 
	// ...
}


// Called when the game starts
void UVR_GrabComponent::BeginPlay()
{
	Super::BeginPlay();
	SetShouldSimulateDrop();
	SetPrimativeCompPhysics(true);
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
	if (isHeld)
	{
		GEngine->AddOnScreenDebugMessage(0, 1, FColor::Cyan, FString::Printf(TEXT("is Held TRUE")));
	}
	else
	{

		GEngine->AddOnScreenDebugMessage(0, 1, FColor::Cyan, FString::Printf(TEXT("is Held FALSE")));
	}
	// ...
}



void UVR_GrabComponent::HandleOnGrabbed()
{
	if (m_OnGrabbed.ExecuteIfBound())
	{
		GEngine->AddOnScreenDebugMessage(0, 2.f, FColor::Emerald, TEXT("On Grabbed Delegate Called"));
	}
}

void UVR_GrabComponent::HandleOnDropped()
{

	if (m_OnDropped.ExecuteIfBound())
	{
		GEngine->AddOnScreenDebugMessage(0, 2.f, FColor::Emerald, TEXT("On Dropped Delegate Called"));
	}
}



bool UVR_GrabComponent::TryGrab(UMotionControllerComponent* MotionController)
{
	GEngine->AddOnScreenDebugMessage(2, 10, FColor::Blue, TEXT("I Am in Try Grab"));

	switch (m_GrabType)
	{
	case GrabType::None:
		break;
	case GrabType::Free:
	{
		SetPrimativeCompPhysics(false);
		AttachParentToController(MotionController);
		isHeld = true;
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Blue, TEXT("Held Free"));

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
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Blue, TEXT("Held Snap"));


		break;
	}
	case GrabType::Custom:
	{
		isHeld = true;
		break;
	}
	}

	if (isHeld == false)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Blue, TEXT("Not Held"));
		return false; // basically says that the object is not held by the user
	}
	MotionControllerReference = MotionController; // assigns a reference to the motion controller

	// the on grabbed delegate needs called here
	HandleOnGrabbed();

	if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		PC->PlayHapticEffect(OnGrabHapticFeedback, GetHeldByHand(), 1, false);  // plays the haptic feedback effect on the controller

	}
	GEngine->AddOnScreenDebugMessage(-0, 1, FColor::Blue, TEXT("Held"));

	return true;
}

bool UVR_GrabComponent::TryRelease()
{
	switch (m_GrabType)
	{
	case GrabType::None:
		break;
	case GrabType::Custom:
		isHeld = false;

		break;
	case GrabType::Free:
		if (isSimulatedOnDrop)
		{
			SetPrimativeCompPhysics(true);
			GEngine->AddOnScreenDebugMessage(50, 20, FColor::Silver, TEXT("Simulated Physics"));

		}
		else
		{
			GetAttachParent()->DetachFromComponent(*detachRules);
			GEngine->AddOnScreenDebugMessage(64, 10, FColor::Green, TEXT("Released From Hand AAAAAAAAAAAAAAAAAAAAAAAA"));

		}

		isHeld = false;

		break;
	}
	if (isHeld == true)
	{
		return false;
	}
	else
	{
		//HandleOnDropped();
		isHeld = false;
		return true;
	}
}

void UVR_GrabComponent::SetShouldSimulateDrop()
{
	if (UPrimitiveComponent* PrimComponent = Cast<UPrimitiveComponent>(GetAttachParent()))
	{
		if (PrimComponent->IsAnySimulatingPhysics())
		{
			isSimulatedOnDrop = true;
		}
	}
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
		GEngine->AddOnScreenDebugMessage(0, 2.f, FColor::Cyan, TEXT("Left Hand"));
		return EControllerHand::Left;  // returns the value of the left hand 
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(0, 2.f, FColor::Cyan, TEXT("Right Hand"));
		return EControllerHand::Right; // returns the value of the right hand
	}
}

void UVR_GrabComponent::AttachParentToController(UMotionControllerComponent* MotionController)
{

	if (GetAttachParent()->AttachToComponent(MotionController, FAttachmentTransformRules::KeepWorldTransform, FName(TEXT("None"))))
	{
		GEngine->AddOnScreenDebugMessage(10, 10, FColor::Yellow, TEXT("I am Attached"));
	}
}

