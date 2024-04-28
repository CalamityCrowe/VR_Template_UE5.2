// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable_Stungun.h"
#include "Components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Cordelias_Sight/VR_Components/VR_GrabComponent.h"

#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"

#include "Cordelias_Sight/Player/Inputs/InputConfigData.h"
#include "Cordelias_Sight/VR_Components/VR_GrabComponent.h"


AInteractable_Stungun::AInteractable_Stungun() :ABase_Interactable()
{


	FireDirection = CreateOptionalDefaultSubobject<UArrowComponent>(TEXT("Fire Direction"));
	FireDirection->SetupAttachment(GetMesh());
	FireDirection->SetRelativeLocation(FVector(0.0f, -3.0f, 5.0f));
	FireDirection->SetRelativeRotation(FRotator(40, 270, 0));

	//LoadDataTable(EObjectType::Gun);
	//LoadHapticEffects(EObjectType::Gun);


}



void AInteractable_Stungun::BeginPlay()
{
	Super::BeginPlay();

	GetGrabComponent()->OnGrabbedDelegate.AddUObject(this, &AInteractable_Stungun::BindInteractableInput);
	GetGrabComponent()->OnDroppedDelegate.AddUObject(this, &AInteractable_Stungun::UnbindInput);
}

void AInteractable_Stungun::FireStunGun()
{

	FHitResult hitResult;

	UE_LOG(LogTemp, Error, TEXT("FIRE STARTED"));

	if (LineTraceMethod(hitResult, FireDirection->GetComponentLocation(), FireDirection->GetForwardVector() * 1000))
	{
		UE_LOG(LogTemp, Error, TEXT("HIT"));
		GEngine->AddOnScreenDebugMessage(110, 2, FColor::Red, TEXT("HIT"));

	}
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		if (GetGrabComponent()->GetHeldByHand() == EControllerHand::Left || GetGrabComponent()->GetHeldByHand() == EControllerHand::Right)
		{
			PC->PlayHapticEffect(GetGrabComponent()->GetInteractHapticEffect(), GetGrabComponent()->GetHeldByHand(), 1, false);  // plays the haptic feedback effect on the controller
		}
	}

}

void AInteractable_Stungun::BindInteractableInput()
{
	ABase_Interactable::BindInteractableInput();
	if (const APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{

		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PC->InputComponent))
		{
			switch (GetGrabComponent()->GetHeldByHand())
			{
			case EControllerHand::Left:
				UE_LOG(LogTemp, Warning, TEXT("LEFT Fire Bound"));
				EnhancedInputComponent->BindAction(m_FireActions->InputLeftTrigger, ETriggerEvent::Triggered, this, &AInteractable_Stungun::FireStunGun);
				break;
			case EControllerHand::Right:
				if (m_FireActions->InputRightTrigger != nullptr)
				{
					UE_LOG(LogTemp, Warning, TEXT("RIGHT Fire Bound"));
					EnhancedInputComponent->BindAction(m_FireActions->InputRightTrigger, ETriggerEvent::Triggered, this, &AInteractable_Stungun::FireStunGun);
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("RIGHT Fire not bound"));
				}
				break;
			}


		}
	}
}

void AInteractable_Stungun::UnbindInput()
{
	ABase_Interactable::UnbindInput();

}

bool AInteractable_Stungun::LineTraceMethod(FHitResult& OutHit, const FVector& StartLocation, const FVector& EndLocation)
{
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);


	DrawDebugLine(GetWorld(), StartLocation, StartLocation + EndLocation, FColor(128, 0, 128), false, 4.f, 10.f, 5.f);

	return (GetWorld()->LineTraceSingleByChannel(OutHit, StartLocation, StartLocation + EndLocation, ECC_Visibility, CollisionParams)); // performs a line trace between the starting point and where it should end

}

