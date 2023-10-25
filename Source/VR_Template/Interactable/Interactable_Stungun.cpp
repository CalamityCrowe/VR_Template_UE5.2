// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable_Stungun.h"
#include "Components/ArrowComponent.h"
#include "VR_Template/Entity/Base_Entity.h"
#include "Kismet/GameplayStatics.h"
#include "VR_Template/VR_Components/VR_GrabComponent.h"


AInteractable_Stungun::AInteractable_Stungun()
{
	m_FireDirection = CreateOptionalDefaultSubobject<UArrowComponent>(TEXT("Fire Direction"));
	m_FireDirection->SetupAttachment(GetMesh());
	m_FireDirection->SetRelativeLocation(FVector(0.0f, -3.0f, 5.0f));
	m_FireDirection->SetRelativeRotation(FRotator(40, 270, 0));

	LoadDataTable(EObjectType::Gun);

}



void AInteractable_Stungun::BeginPlay()
{
	Super::BeginPlay();
	//FireStunGun();
}

void AInteractable_Stungun::FireStunGun()
{
#if WITH_EDITOR
	GEngine->AddOnScreenDebugMessage(110, 2, FColor::Red, TEXT("AAAAAAAAAAAAA"));
#endif 
	if (FHitResult* newResult = LineTraceMethod(m_FireDirection->GetComponentLocation(), m_FireDirection->GetForwardVector() * 1000))
	{
#if WITH_EDITOR
		GEngine->AddOnScreenDebugMessage(110, 2, FColor::Red, TEXT("HIT"));
#endif
		if (ABase_Entity* newEntity = Cast<ABase_Entity>(newResult->GetActor()))
		{
			// do stuff
		}
	}
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		PC->PlayHapticEffect(GetGrabComponent()->GetInteractHapticEffect(), GetGrabComponent()->GetHeldByHand(), 1, false);  // plays the haptic feedback effect on the controller
	}

}

FHitResult* AInteractable_Stungun::LineTraceMethod(const FVector& StartLocation, const FVector& EndLocation)
{
	FHitResult* HitResult = new FHitResult();
	FCollisionQueryParams CollisionParams;


#if WITH_EDITOR
	DrawDebugLine(GetWorld(), StartLocation, StartLocation + EndLocation, FColor(128, 0, 128), false, 4.f, 10.f, 5.f);
#endif

	if (GetWorld()->LineTraceSingleByChannel(*HitResult, StartLocation, StartLocation + EndLocation, ECC_Visibility, CollisionParams)) // performs a line trace between the starting point and where it should end
	{
		// a hit occurred
		return HitResult;
	}

	return HitResult;
}

