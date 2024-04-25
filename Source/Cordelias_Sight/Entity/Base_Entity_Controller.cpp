// Fill out your copyright notice in the Description page of Project Settings.


#include "Base_Entity_Controller.h"
#include "Base_Entity.h"
#include "NavigationSystem.h"

ABase_Entity_Controller::ABase_Entity_Controller()
{

}

void ABase_Entity_Controller::BeginPlay()
{
	Super::BeginPlay();

	if (ControlledPawnReference == nullptr)
	{
		if (Cast<ABase_Entity>(GetPawn()))
		{
			ControlledPawnReference = Cast<ABase_Entity>(GetPawn());
			GEngine->AddOnScreenDebugMessage(0, 2, FColor::Magenta, TEXT("Pawn Reference Found"));
		}
	}
}

void ABase_Entity_Controller::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (TargetActorReference)
	{
		MoveActor(TargetActorReference);
	}

}

void ABase_Entity_Controller::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (InPawn)
	{
		GEngine->AddOnScreenDebugMessage(0, 2, FColor::Magenta, TEXT("Possessed"));
	}
}

void ABase_Entity_Controller::MoveActor(AActor* TargetActor)
{
	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
	if (ControlledPawnReference) // if the pawn reference is valid
	{
		const ANavigationData* NavData = NavSys->GetDefaultNavDataInstance(FNavigationSystem::DontCreate); //gets the navigation data
		MoveToActor(TargetActor, 0); // moves to the target actor
	}
}

