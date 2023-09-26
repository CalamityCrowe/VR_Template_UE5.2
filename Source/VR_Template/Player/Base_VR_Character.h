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

protected:
	UPROPERTY(Category = "Motion Controllers", VisibleAnywhere, BlueprintReadOnly)
		TObjectPtr<class UMotionControllerComponent> m_RightController;
	UPROPERTY(Category = "Motion Controllers", VisibleAnywhere, BlueprintReadOnly)
		TObjectPtr<class UMotionControllerComponent> m_LeftController;
	UPROPERTY(Category = "Camera", VisibleAnywhere, BlueprintReadOnly)
		TObjectPtr<class UCameraComponent> m_Camera;
};
