// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Base_VR_Character.generated.h"



UCLASS()
class VR_TEMPLATE_API ABase_VR_Character : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABase_VR_Character();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	UFUNCTION()
		void MovePlayer(const FInputActionValue& Value);
	UFUNCTION()
		void TurnPlayer(const FInputActionValue& Value);

protected:


	UPROPERTY(Category = "Camera", VisibleAnywhere, BlueprintReadOnly)
		TObjectPtr<class UCameraComponent> m_Camera;

	UPROPERTY(EditAnywhere, Category = "Enhanced Input")
		TObjectPtr<class UInputMappingContext> m_InputMappingContext;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
		TObjectPtr<class UInputConfigData> m_InputActions; // this is a custom data config that holds a list of pointers that will get assigned in the engine to pull the input actions in

	UPROPERTY(Category = "Motion Controllers", VisibleAnywhere, BlueprintReadOnly)
		TObjectPtr<class UMotionControllerComponent> m_RightController;
	UPROPERTY(Category = "Motion Controllers", VisibleAnywhere, BlueprintReadOnly)
		TObjectPtr<UMotionControllerComponent> m_LeftController;
	UPROPERTY(Category = "Motion Controllers", EditDefaultsOnly, BlueprintReadOnly)
		TObjectPtr<class UMannequin_Hands> m_HandRight;
	UPROPERTY(Category = "Motion Controllers", EditDefaultsOnly, BlueprintReadOnly)
		TObjectPtr<UMannequin_Hands> m_HandLeft;
	UPROPERTY(Category = "Motion Controllers", EditDefaultsOnly, BlueprintReadOnly)
		TObjectPtr<class UVR_GrabComponent> m_HeldRight;
	UPROPERTY(Category = "Motion Controllers", EditDefaultsOnly, BlueprintReadOnly)
		TObjectPtr<UVR_GrabComponent> m_HeldLeft;

};
