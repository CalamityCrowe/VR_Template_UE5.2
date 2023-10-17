// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseVrGamemode.h"
#include "Base_VR_Character.h"
#include "BaseVRGameState.h"


ABaseVrGamemode::ABaseVrGamemode()
{
	auto player = ConstructorHelpers::FObjectFinder<ABase_VR_Character>(TEXT("Blueprint'/Game/My_VR_Character/PlayerCharacter.PLayerCharacter'"));

	GameStateClass = ABaseVRGameState::StaticClass();
}

void ABaseVrGamemode::StartPlay()
{
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Purple, TEXT("Start Play Has Been Called"));
	Super::StartPlay();
}
