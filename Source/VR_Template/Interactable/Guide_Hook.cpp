// Fill out your copyright notice in the Description page of Project Settings.


#include "Guide_Hook.h"
#include "VR_Template/SplineActor.h"
#include "Kismet/GameplayStatics.h"
#include "VR_Template/VR_Components/VR_GrabComponent.h"


AGuide_Hook::AGuide_Hook()
{
	LoadDataTable(EObjectType::Moveable);
	LoadHapticEffects(EObjectType::Moveable);
}

void AGuide_Hook::BeginPlay()
{
	Super::BeginPlay();

	GetMesh()->OnComponentBeginOverlap.AddDynamic(this, &AGuide_Hook::OnBeginOverlap); 

	StartLocation = GetActorLocation();
}


void AGuide_Hook::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGuide_Hook::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ASplineActor* tempSpline = Cast<ASplineActor>(OtherActor))
	{

		SetActorLocation(StartLocation);
		if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
		{
			PC->PlayHapticEffect(GetGrabComponent()->GetInteractHapticEffect(), GetGrabComponent()->GetHeldByHand(), 1, false);  // plays the haptic feedback effect on the controller
		}

	}

}