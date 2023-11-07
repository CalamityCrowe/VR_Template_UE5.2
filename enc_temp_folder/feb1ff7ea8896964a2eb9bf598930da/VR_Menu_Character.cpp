// Fill out your copyright notice in the Description page of Project Settings.


#include "VR_Menu_Character.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Inputs/MenuInputData.h"

AVR_Menu_Character::AVR_Menu_Character()
{
	//auto MappingContextData = ConstructorHelpers::FObjectFinder<UInputMappingContext>(TEXT("InputMappingContext'/Game/Enhanced_Input_Settings/InputMappings/IMC_Menu.IMC_Menu'"));
	//InputMappingContext = MappingContextData.Object;

	//auto InputData = ConstructorHelpers::FObjectFinder<UDataAsset>(TEXT("MenuInputData'/Game/Enhanced_Input_Settings/InputMappings/VR_Menu_Inputs.VR_Menu_Inputs'"));
	//InputActions = InputData.Object;
}

void AVR_Menu_Character::BeginPlay()
{
	Super::BeginPlay();

}

void AVR_Menu_Character::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AVR_Menu_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


	if (APlayerController* PC = Cast<APlayerController>(GetController())) // tries to grab an instance of the player controller
	{
		if (ULocalPlayer* LocalPlayer = PC->GetLocalPlayer()) // gets the local player from the controller
		{
			if (UEnhancedInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())  // gets the enhanced input from the local input system
			{
				InputSystem->ClearAllMappings(); // clears any mapping contexts if they had been assigned by mistake
				InputSystem->AddMappingContext(InputMappingContext, 0); // assigns the mapping contexts as the highest priority
			}
		}
	}



}
