// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable_Stungun.h"
#include "Components/ArrowComponent.h"
#include "VR_Template/Entity/Base_Entity.h"
#include "Kismet/GameplayStatics.h"
#include "VR_Template/VR_Components/VR_GrabComponent.h"


AInteractable_Stungun::AInteractable_Stungun()
{
	FireDirection = CreateOptionalDefaultSubobject<UArrowComponent>(TEXT("Fire Direction"));
	FireDirection->SetupAttachment(GetMesh());
	FireDirection->SetRelativeLocation(FVector(0.0f, -3.0f, 5.0f));
	FireDirection->SetRelativeRotation(FRotator(40, 270, 0));

	LoadDataTable(EObjectType::Gun);

}



void AInteractable_Stungun::BeginPlay()
{
	Super::BeginPlay();
	FireStunGun();
}

void AInteractable_Stungun::FireStunGun()
{

	FHitResult hitResult;
#if WITH_EDITOR
	GEngine->AddOnScreenDebugMessage(110, 2, FColor::Red, TEXT("AAAAAAAAAAAAA"));
#endif 
	if (LineTraceMethod(hitResult, FireDirection->GetComponentLocation(), FireDirection->GetForwardVector() * 1000))
	{
#if WITH_EDITOR
		GEngine->AddOnScreenDebugMessage(110, 2, FColor::Red, TEXT("HIT"));
#endif
		if (ABase_Entity* newEntity = Cast<ABase_Entity>(hitResult.GetActor()))
		{
			// do stuff
		}
	}
	//if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	//{
	//	PC->PlayHapticEffect(GetGrabComponent()->GetInteractHapticEffect(), GetGrabComponent()->GetHeldByHand(), 1, false);  // plays the haptic feedback effect on the controller
	//}

}

bool AInteractable_Stungun::LineTraceMethod(FHitResult& OutHit, const FVector& StartLocation, const FVector& EndLocation)
{
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

#if WITH_EDITOR
	DrawDebugLine(GetWorld(), StartLocation, StartLocation + EndLocation, FColor(128, 0, 128), false, 4.f, 10.f, 5.f);
#endif

	return (GetWorld()->LineTraceSingleByChannel(OutHit, StartLocation, StartLocation + EndLocation, ECC_Visibility, CollisionParams)); // performs a line trace between the starting point and where it should end

}

