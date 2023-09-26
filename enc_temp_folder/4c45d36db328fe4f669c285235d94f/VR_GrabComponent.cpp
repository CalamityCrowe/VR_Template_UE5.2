// Fill out your copyright notice in the Description page of Project Settings.


#include "VR_GrabComponent.h"

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

	// ...
	
}


// Called every frame
void UVR_GrabComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UVR_GrabComponent::TryGrab()
{
}

void UVR_GrabComponent::TryRelease()
{
}

void UVR_GrabComponent::SetShouldSimulateDrop()
{
}

void UVR_GrabComponent::SetPrimativeCompPhysics()
{
}

void UVR_GrabComponent::GetHeldByHand()
{
}

