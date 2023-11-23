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
}

void ABase_Entity_Controller::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime); 
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
	if(APawn* ControlledPawn = GetPawn())
	{
		const ANavigationData* NavData = NavSys->GetDefaultNavDataInstance(FNavigationSystem::DontCreate);
		if(ABase_Entity* CurrentEntity = Cast<ABase_Entity>(ControlledPawn))
		{
			MoveToActor(TargetActor, 0); 
		}
	}
}

