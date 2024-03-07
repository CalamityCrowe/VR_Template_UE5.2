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
private:
#pragma region Function Decleration
	void AllignColliderToHMD();
	UFUNCTION()
	void VerticalMovement(const FInputActionValue& Value);
	UFUNCTION()
	void HorizontalMovement(const FInputActionValue& Value);
	UFUNCTION()
	void TurnPlayer(const FInputActionValue& Value);
	UFUNCTION()
	void GrabObjectLeft(const FInputActionValue& Value);
	UFUNCTION()
	void GrabObjectRight(const FInputActionValue& Value);
	UFUNCTION()
	void ReleaseObjectLeft(const FInputActionValue& Value);
	UFUNCTION()
	void ReleaseObjectRight(const FInputActionValue& Value);
#pragma endregion



#pragma region Variable Decleration
	UPROPERTY(Category = "Camera", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UCameraComponent> Camera;


	UPROPERTY(Category = "VR Orign", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<class USceneComponent> VROrigin;
	UPROPERTY(Category = "Motion Controllers", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UMotionControllerComponent> RightController;
	UPROPERTY(Category = "Motion Controllers", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UMotionControllerComponent> LeftController;
	UPROPERTY(Category = "Motion Controllers", EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UMannequin_Hands> HandRight;
	UPROPERTY(Category = "Motion Controllers", EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UMannequin_Hands> HandLeft;
	UPROPERTY(Category = "Motion Controllers", EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UVR_GrabComponent> HeldRight;
	UPROPERTY(Category = "Motion Controllers", EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UVR_GrabComponent> HeldLeft;
#pragma endregion

	UPROPERTY(Category = "Movement", EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	float WalkSpeedScale;
	UPROPERTY(Category = "Movement", EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	float TurnScale; 

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

#pragma region Variable Decleration
	UPROPERTY(EditAnywhere, Category = "Enhanced Input")
	class UInputMappingContext* InputMappingContext;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
	class UDataAsset* InputActions; // this is a custom data config that holds a list of pointers that will get assigned in the engine to pull the input actions in

	UPROPERTY(EditDefaultsOnly, BluePrintReadWrite, Category = "Motion Controllers")
	float GrabRadius;
	UPROPERTY(EditDefaultsOnly, BluePrintReadWrite, Category = "Motion Controllers")
	float LocalNearestDistance;
#pragma endregion

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	class UVR_GrabComponent* GetGrabComponentNearController(UMotionControllerComponent* controllerReference);



};
