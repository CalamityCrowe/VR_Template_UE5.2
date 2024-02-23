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
		Mesh->SetVisibility(false);
		Collider->Deactivate();
		AudioPlayer->Stop();
		NiagaraComponent->Deactivate();
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


	if (bMoveable && SplineReference && bActive)
	{

		Time = (Time >= 1) ? 0 : Time += (DeltaTime / InterpSpeed);
		FVector newLocation = SplineReference->GetSplineMesh()->GetLocationAtTime(Time, ESplineCoordinateSpace::World);
		SetActorLocation(newLocation);
		//if (abs(SplineReference->GetSplineMesh()->GetTransformAtSplinePoint(CurrentSplineIndex, ESplineCoordinateSpace::World).GetLocation().Length()) - abs(GetActorLocation().Length()) == 0)
		//{
		//	CurrentSplineIndex++;
		//	if (CurrentSplineIndex > SplineReference->GetSplineMesh()->GetNumberOfSplinePoints())
		//	{
		//		CurrentSplineIndex = 0;
		//	}
		//}
		//else
		//{
		//	FVector CurrentLocation = GetActorLocation();
		//	FVector TargetLocation = SplineReference->GetSplineMesh()->GetTransformAtSplinePoint(CurrentSplineIndex, ESplineCoordinateSpace::World).GetLocation();
		//	FVector newLoc = UKismetMathLibrary::VInterpTo_Constant(CurrentLocation, TargetLocation, DeltaTime, InterpSpeed);
		//	SetActorLocation(newLoc);
		//}
	}

}


void AAudio_Actor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ABase_VR_Character* tempPlayer = Cast<ABase_VR_Character>(OtherActor))
	{
		if (bLast == false)
		{
			NextActor->ActivateActor();
		}
		Destroy();
	}
}

/// <summary>
/// This handle the activation of the next actor references that the current actor is connected to.
/// 
/// the main things it will do is set the visibility of the mesh to true, activate the collider and the audio player. 
/// </summary>
void AAudio_Actor::ActivateActor()
{
	Mesh->SetVisibility(true);
	Collider->Activate(true);
	AudioPlayer->Activate(true);
	NiagaraComponent->Activate(true);
	bActive = true;
}

