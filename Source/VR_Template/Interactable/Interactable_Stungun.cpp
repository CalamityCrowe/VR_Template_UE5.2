// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable_Stungun.h"
#include "Components/ArrowComponent.h"
#include "VR_Template/Entity/Base_Entity.h"

AInteractable_Stungun::AInteractable_Stungun()
{
	auto MeshAsset = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Game/Models/Glock-17.Glock-17'"));
	GetMesh()->SetStaticMesh(MeshAsset.Object);
}



void AInteractable_Stungun::BeginPlay()
{
	Super::BeginPlay();
	FireStunGun();
}

void AInteractable_Stungun::FireStunGun()
{

	GEngine->AddOnScreenDebugMessage(110, 2, FColor::Red, TEXT("AAAAAAAAAAAAA"));
	if (FHitResult* newResult = LineTraceMethod(FVector(0, 0, 0), FVector(2000, 2000, 0)))
	{
		GEngine->AddOnScreenDebugMessage(110, 2, FColor::Red, TEXT("HIT"));
		if (ABase_Entity* newEntity = Cast<ABase_Entity>(newResult->GetActor())) 
		{
			// do stuff
		}
	}
}

FHitResult* AInteractable_Stungun::LineTraceMethod(const FVector& StartLocation, const FVector& EndLocation)
{
	FHitResult* HitResult = new FHitResult();
	FCollisionQueryParams CollisionParams;

	if (GetWorld()->LineTraceSingleByChannel(*HitResult, StartLocation, EndLocation, ECC_Visibility, CollisionParams)) // performs a line trace between the starting point and where it should end
	{
		// a hit occurred
		return HitResult;
	}

	return HitResult;
}

