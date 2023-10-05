// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseVrGamemode.h"
#include "Base_VR_Character.h"
#include "BaseVRGameState.h"


ABaseVrGamemode::ABaseVrGamemode()
{
	TSoftClassPtr<ABase_VR_Character> ActorBpClass = TSoftClassPtr<ABase_VR_Character>(FSoftObjectPath(TEXT("Blueprint'/Game/My_VR_Character/PlayerCharacter'")));

	DefaultPawnClass = ActorBpClass->StaticClass();
	GameStateClass = ABaseVRGameState::StaticClass(); 
}

void ABaseVrGamemode::StartPlay()
{
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Purple, TEXT("Start Play Has Been Called")); 
	Super::StartPlay(); 
}
