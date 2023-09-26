// Fill out your copyright notice in the Description page of Project Settings.


#include "Base_VR_Character.h"

#include "MotionControllerComponent.h" // this is used in order to get the motion controllers in the game
#include "Camera/CameraComponent.h" // gets the camera for using in the game
#include "GameFramework/CharacterMovementComponent.h" // gets the functionallity of the movement component

// Sets default values
ABase_VR_Character::ABase_VR_Character()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	m_LeftController = CreateOptionalDefaultSubobject<UMotionControllerComponent>(TEXT("Left Motion Controller"));
	m_RightController = CreateOptionalDefaultSubobject<UMotionControllerComponent>(TEXT("Right Motion Controller"));

	m_LeftController->MotionSource = FName(TEXT("Left"));
	m_RightController->MotionSource = FName(TEXT("Right"));


}

// Called when the game starts or when spawned
void ABase_VR_Character::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABase_VR_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABase_VR_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

