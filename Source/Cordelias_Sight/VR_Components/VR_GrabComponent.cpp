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

	bisHeld = false;


	grabType = GrabTypes::None;
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
#if WITH_EDITOR
	if (bisHeld)
	{
		GEngine->AddOnScreenDebugMessage(0, 1, FColor::Cyan, FString::Printf(TEXT("is Held TRUE")));
	}
	else
	{

		GEngine->AddOnScreenDebugMessage(0, 1, FColor::Cyan, FString::Printf(TEXT("is Held FALSE")));
	}
#endif
}



void UVR_GrabComponent::HandleOnGrabbed()
{
	// calls the delegate 


	if (OnGrabbedDelegate.IsBound())
	{
		OnGrabbedDelegate.Broadcast();
		UE_LOG(LogTemp, Warning, TEXT("On Grabbed Delegate Called"));
	}
}

void UVR_GrabComponent::HandleOnDropped()
{

	if (OnDroppedDelegate.IsBound())
	{
		OnDroppedDelegate.Broadcast();
		UE_LOG(LogTemp, Warning, TEXT("On Dropped Delegate Called"));
	}
}



bool UVR_GrabComponent::TryGrab(UMotionControllerComponent* MotionController)
{
	UE_LOG(LogTemp, Warning, TEXT("Try Grab Called"));
	switch (grabType)
	{
	case GrabTypes::None:
		break;
	case GrabTypes::Free:
	{
		SetPrimativeCompPhysics(false);
		AttachParentToController(MotionController);
		bisHeld = true;
		UE_LOG(LogTemp, Warning, TEXT("Held Free"));
		break;

	}
	case GrabTypes::Snap:
	{
		SetPrimativeCompPhysics(false);
		AttachParentToController(MotionController);
		bisHeld = true;

		const FRotator invertedRotation = UKismetMathLibrary::NegateRotator(GetRelativeRotation());

		SetRelativeRotation(invertedRotation); // sets the rotation of the object to the inverse of the rotation of the object

		const FVector ParentLocation = GetAttachParent()->GetComponentLocation(); // gets the location of the parent

		FVector newLocation = (GetComponentLocation() - ParentLocation) * -1;

		newLocation = MotionController->GetComponentLocation() + newLocation; // sets the new location based on the motion controllers location + an offset

		SetWorldLocation(newLocation); // sets the world location of the object to the new location

		//SetWorldLocation(newLocation, false, nullptr, ETeleportType::TeleportPhysics); // sets the world location of the object to the new location
		UE_LOG(LogTemp, Warning, TEXT("Held Snap"));

		break;
	}
	case GrabTypes::Custom:
	{
		bisHeld = true;
		break;
	}
	}

	if (bisHeld == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("Not Held"));
		return false; // basically says that the object is not held by the user
	}
	MotionControllerReference = MotionController; // assigns a reference to the motion controller

	// the on grabbed delegate needs called here
	HandleOnGrabbed();

	if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		PC->PlayHapticEffect(OnGrabHapticFeedback, GetHeldByHand(), 1, false);  // plays the haptic feedback effect on the controller
		UE_LOG(LogTemp, Warning, TEXT("Haptic Effect Played"));

	}
	UE_LOG(LogTemp, Warning, TEXT("Held"));
	return true;
}

bool UVR_GrabComponent::TryRelease()
{
	switch (grabType)
	{
	case GrabTypes::None:
		break;
	case GrabTypes::Custom:
		bisHeld = false;

		break;
	case GrabTypes::Snap:
	case GrabTypes::Free:
		if (bisSimulatedOnDrop)
		{
			SetPrimativeCompPhysics(true);
			UE_LOG(LogTemp, Warning, TEXT("Released From Hand"));
		}
		else
		{
			GetAttachParent()->DetachFromComponent(*detachRules);
			UE_LOG(LogTemp, Warning, TEXT("Released From Hand"));
		}

		bisHeld = false;

		break;
	default:
		break;
	}
	if (bisHeld == true)
	{
		return false;
	}
	else
	{
		HandleOnDropped();
		bisHeld = false;
		return true;
	}
}

void UVR_GrabComponent::SetShouldSimulateDrop()
{

	if (UPrimitiveComponent* PrimComponent = Cast<UPrimitiveComponent>(GetAttachParent()))
	{
		if (PrimComponent->IsAnySimulatingPhysics())
		{
			bisSimulatedOnDrop = true;
		}
	}
}

void UVR_GrabComponent::SetPrimativeCompPhysics(bool isSimulated)
{
	if (UPrimitiveComponent* PrimComponent = Cast<UPrimitiveComponent>(GetAttachParent()))
	{
		PrimComponent->SetSimulatePhysics(isSimulated);  // sets the physics of the component
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No Primitive Component Found For Setting Comp Physics"));
	}

}

EControllerHand UVR_GrabComponent::GetHeldByHand()
{
	if (MotionControllerReference->MotionSource == FName(TEXT("Left"))) // checks if it is the left hand that has grabbed the object
	{
		UE_LOG(LogTemp, Warning, TEXT("Left Hand"));
		return EControllerHand::Left;  // returns the value of the left hand 
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Right Hand"));
		return EControllerHand::Right; // returns the value of the right hand
	}
}

void UVR_GrabComponent::AttachParentToController(UMotionControllerComponent* MotionController)
{

	if (GetAttachParent()->AttachToComponent(MotionController, FAttachmentTransformRules::KeepWorldTransform, FName(TEXT("None"))))
	{
		UE_LOG(LogTemp, Warning, TEXT("Attached To Controller"));

	}
}

