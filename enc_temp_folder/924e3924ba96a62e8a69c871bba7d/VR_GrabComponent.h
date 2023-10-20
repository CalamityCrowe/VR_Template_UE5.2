// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "VR_GrabComponent.generated.h"


// NEED TO DECLARE A CUSTOM EVENT DISPATCHER FOR THE ON GRABBED AND DROPPED FUNCTIONS
DECLARE_DELEGATE(FOnGrabbedDelegate);
DECLARE_DELEGATE(FOnDroppedDelegate);

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

	void SetShouldSimulateDrop();
	void SetPrimativeCompPhysics(bool isSimulated);


	void AttachParentToController(class UMotionControllerComponent* MotionController);

public: //functions
	UFUNCTION(BlueprintPure)
		EControllerHand GetHeldByHand();
private: 
	FDetachmentTransformRules* detachRules;

protected: // variables

	bool isHeld;
	FRotator PrimaryGrabRelativerRotation;
	bool isSimulatedOnDrop;
	GrabType m_GrabType;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Haptic Feedback")
		TObjectPtr<class UHapticFeedbackEffect_Base> OnGrabHapticFeedback; // this gets assigned in the engine and helps register that the player has interacted in the world

	TObjectPtr<UMotionControllerComponent> MotionControllerReference;
public: //functions
	UFUNCTION()
		void HandleOnGrabbed();
	UFUNCTION()
		void HandleOnDropped();

	bool TryGrab(class UMotionControllerComponent* MotionCOntroller);
	bool TryRelease();

	void SetGrabType(GrabType newType) { m_GrabType = newType; }
	GrabType GetGrabType() { return m_GrabType; }
public: //variables 
	FOnGrabbedDelegate m_OnGrabbed;
	FOnDroppedDelegate m_OnDropped;

	UFUNCTION(BlueprintImplementableEvent)
	void IEATGLUE(); 
};
