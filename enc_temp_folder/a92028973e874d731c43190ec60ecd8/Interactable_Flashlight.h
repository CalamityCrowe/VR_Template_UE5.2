// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base_Interactable.h"
#include "Interactable_Flashlight.generated.h"

class USpotLightComponent;
/**
 *
 */
UCLASS()
class VR_TEMPLATE_API AInteractable_Flashlight : public ABase_Interactable
{
	GENERATED_BODY()
public:
	AInteractable_Flashlight();
	virtual ~AInteractable_Flashlight() = default;
protected:
	virtual void BeginPlay() override; 
public:
	virtual void Tick(float) override; 
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Light", meta = (AllowPrivateAccess = true))
	TObjectPtr<USpotLightComponent> m_Light;
public:
	USpotLightComponent* GetLight() { return m_Light; }
protected:
	void ToggleFlashlight(); 


};
