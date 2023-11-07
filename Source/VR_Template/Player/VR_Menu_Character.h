// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base_VR_Character.h"
#include "VR_Menu_Character.generated.h"

/**
 * 
 */
UCLASS()
class VR_TEMPLATE_API AVR_Menu_Character : public ABase_VR_Character
{
	GENERATED_BODY()
public:
	AVR_Menu_Character(); 

protected:
	virtual void BeginPlay() override;
public:
	virtual void Tick(float DeltaSeconds) override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;



};
