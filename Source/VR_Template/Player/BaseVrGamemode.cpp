// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseVrGamemode.h"
#include "Base_VR_Character.h"
#include "BaseVRGameState.h"
#include "GameFramework/PlayerController.h"


ABaseVrGamemode::ABaseVrGamemode()
{
	//static ConstructorHelpers::FClassFinder<APawn> player(TEXT("/Game/My_VR_Character/PlayerCharacter"));


	////Script/Engine.Blueprint'/Game/My_VR_Character/PlayerCharacter.PlayerCharacter'
	//if (player.Class != nullptr)
	//{
	//	DefaultPawnClass = player.Class;
	//}
	//else
	//{
	//	UE_LOG(LogTemp,Error,TEXT("Player Character not found"))
	//}
	//auto player = ConstructorHelpers::FObjectFinder<ABase_VR_Character>(TEXT("Blueprint'/Game/My_VR_Character/PlayerCharacter.PLayerCharacter'"));

	//GameStateClass = ABaseVRGameState::StaticClass();
}

void ABaseVrGamemode::StartPlay()
{
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Purple, TEXT("Start Play Has Been Called"));
	Super::StartPlay();
}

void ABaseVrGamemode::Tick(float Delta)
{
	Super::Tick(Delta);

	// Get the player controller

}
