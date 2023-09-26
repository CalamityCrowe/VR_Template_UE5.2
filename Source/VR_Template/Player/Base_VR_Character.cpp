// Fill out your copyright notice in the Description page of Project Settings.


#include "Base_VR_Character.h"

// Sets default values
ABase_VR_Character::ABase_VR_Character()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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

