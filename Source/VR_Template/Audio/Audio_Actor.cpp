// Fill out your copyright notice in the Description page of Project Settings.


#include "Audio_Actor.h"

#include "Components/AudioComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SplineComponent.h"
#include "VR_Template/Player/Base_VR_Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "VR_Template/SplineActor.h"
#include "Components/SplineComponent.h"
#include "NiagaraComponent.h"
#include <VR_Template/Level/CustomVRLevelScript.h>
#include "Components/PointLightComponent.h"

// Sets default values
AAudio_Actor::AAudio_Actor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Collider = CreateOptionalDefaultSubobject<UCapsuleComponent>(TEXT("Collider"));
	RootComponent = Collider;

	Mesh = CreateOptionalDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	auto MeshAsset = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Game/Assets/SM_Cube.SM_Cube'"));
	Mesh->SetStaticMesh(MeshAsset.Object);

	AudioPlayer = CreateOptionalDefaultSubobject<UAudioComponent>(TEXT("Audio Player"));
	AudioPlayer->SetupAttachment(RootComponent);

	NiagaraComponent = CreateOptionalDefaultSubobject<UNiagaraComponent>(TEXT("Niagara Component"));
	NiagaraComponent->SetupAttachment(RootComponent);
	NiagaraComponent->SetRelativeLocation(FVector(0, 0, 0));

	bLast = false;
	bActive = false;
	bMoveable = false;

	InterpSpeed = 100;
}

// Called when the game starts or when spawned
void AAudio_Actor::BeginPlay()
{
	Super::BeginPlay();
	if (!bActive)
	{
		Mesh->SetVisibility(false); // sets the visibility of the mesh to false
		Collider->Deactivate(); // deactivates the collider
		AudioPlayer->Stop();	// stops the audio from playing
		NiagaraComponent->Deactivate(); // deactivates the niagara component
	}
	else
	{
		if (AudioPlayer->GetSound()) // checks if there is a sound file set on the audio component
		{
			AudioPlayer->Play(); // plays the audio 
		}
	}
}

// Called every frame
void AAudio_Actor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bMoveable && SplineReference && bActive) // checks if the actor is moveable, has a spline reference and is active
	{
		Time = (Time >= SplineReference->GetSplineMesh()->Duration) ? 0 : Time += (DeltaTime); // checks if the time is greater than the duration of the spline mesh, if it is it sets the time to 0, if not it adds the delta time to the time
		FVector newLocation = SplineReference->GetSplineMesh()->GetLocationAtTime(Time, ESplineCoordinateSpace::World); // gets the location of the spline mesh at the current time
		SetActorLocation(newLocation); // sets the actor location to the new location
	}

}


void AAudio_Actor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ABase_VR_Character* tempPlayer = Cast<ABase_VR_Character>(OtherActor))
	{
		if (bLast == false) // checks if the actor is not the last audio actor
		{
			NextActor->ActivateActor(); // activates the next actor to go to
		}
		else // if the actor is the last audio actor
		{
			if (ACustomVRLevelScript* tempLevel = Cast<ACustomVRLevelScript>(GetLevel())) // checks if the current level is a custom level script
			{
				tempLevel->LevelFinished(); // calls the level finished event in the blueprints
			}
		}
		Destroy(); // destroys the current actor
	}
}

/// <summary>
/// This handle the activation of the next actor references that the current actor is connected to.
/// 
/// the main things it will do is set the visibility of the mesh to true, activate the collider and the audio player. 
/// </summary>
void AAudio_Actor::ActivateActor()
{
	Mesh->SetVisibility(true); // sets the visibility of the mesh to true
	Collider->Activate(true); // activates the collider
	AudioPlayer->Activate(true); // activates the audio player
	NiagaraComponent->Activate(true); // activates the niagara component
	bActive = true; // sets the active boolean to true so it can move if it needs to move
}

