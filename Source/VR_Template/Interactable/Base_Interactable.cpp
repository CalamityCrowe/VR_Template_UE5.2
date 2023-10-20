// Fill out your copyright notice in the Description page of Project Settings.


#include "Base_Interactable.h"
#include "VR_Template/VR_Components/VR_GrabComponent.h"
// Sets default values
ABase_Interactable::ABase_Interactable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_Mesh = CreateOptionalDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh")); 
	RootComponent = m_Mesh; 

	m_GrabPointSnap = CreateOptionalDefaultSubobject<UVR_GrabComponent>(TEXT("Grab Point"));
	m_GrabPointSnap->SetupAttachment(m_Mesh);
	if (m_GrabPointSnap->GetGrabType() == GrabType::None) // checks if there is no grab type set and if so sets it to snap 
	{
		m_GrabPointSnap->SetGrabType(GrabType::Snap); 
	}
}

// Called when the game starts or when spawned
void ABase_Interactable::BeginPlay()
{
	Super::BeginPlay();

	if (m_GrabPointSnap) 
	{
		m_GrabPointSnap->m_OnGrabbed.BindUObject(this, &ABase_Interactable::BindInteractableInput); 
		m_GrabPointSnap->m_OnDropped.BindUObject(this, &ABase_Interactable::UnbindInput); // this is used for binding inputs within interactable objects
	}
	
}

// Called every frame
void ABase_Interactable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABase_Interactable::BindInteractableInput()
{
	OnGrabbed();
	GEngine->AddOnScreenDebugMessage(0, 2.f, FColor::Red, TEXT("Attached Component, Base Interactable.cpp"));
}

void ABase_Interactable::UnbindInput()
{
	OnDropped(); 
	GEngine->AddOnScreenDebugMessage(0, 2.f, FColor::Red, TEXT("Detached Component, Base Interactable.cpp"));
}

