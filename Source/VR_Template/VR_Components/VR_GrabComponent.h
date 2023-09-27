// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "VR_GrabComponent.generated.h"


// NEED TO DECLARE A CUSTOM EVENT DISPATCHER FOR THE ON GRABBED AND DROPPED FUNCTIONS


UENUM()
enum class GrabType : uint8
{
	None UMETA(DisplayName = "None"),
	Free UMETA(DisplayName = "Free"),
	Snap UMETA(DisplayName = "Snap"),
	Custom UMETA(DisplayName = "Custom")

};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class VR_TEMPLATE_API UVR_GrabComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UVR_GrabComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:

	bool TryGrab(class UMotionControllerComponent* MotionCOntroller);
	bool TryRelease();
	void SetShouldSimulateDrop();
	void SetPrimativeCompPhysics(bool isSimulated);
	EControllerHand GetHeldByHand();

	void AttachParentToController(class UMotionControllerComponent* MotionController);

protected:

	bool isHeld;;
	FRotator PrimaryGrabRelativerRotation;
	bool isSimulatedOnDrop;
	GrabType m_GrabType;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Haptic Feedback")
	TObjectPtr<class UHapticFeedbackEffect_Base> OnGrabHapticFeedback; // this gets assigned in the engine and helps register that the player has interacted in the world

	TObjectPtr<UMotionControllerComponent> MotionControllerReference;

public:
	UFUNCTION(BlueprintNativeEvent)
	void OnGrabbed();
	UFUNCTION(BlueprintNativeEvent)
	void OnDropped();

};
