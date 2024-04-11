// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base_VR_Character.h"
#include "VR_Menu_Character.generated.h"

/**
 *
 */

class UWidgetInteractionComponent;
class UNiagaraComponent;
class UInputConfigData;

UCLASS()
class VR_TEMPLATE_API AVR_Menu_Character : public ABase_VR_Character
{
	GENERATED_BODY()
public:
	AVR_Menu_Character();
	~AVR_Menu_Character();
protected:
	virtual void BeginPlay() override;
public:
	virtual void Tick(float DeltaSeconds) override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
protected:
	virtual void VerticalMovement(const FInputActionValue& Value) override;
	virtual void HorizontalMovement(const FInputActionValue& Value) override;
	virtual void TurnPlayer(const FInputActionValue& Value) override;
	virtual void SnapTurn(const FInputActionValue& Value) override;
	virtual void ToggleSnapTurn(const FInputActionValue& Value) override;
private:

	/*
	* to do
	* Take the widget interaction stuff and move it to code
	* add the niagara system  to each of the hands
	* have it render from each hand correctly
	*
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Widget Interaction", meta = (AllowPrivateAccess = true))
	TObjectPtr<UWidgetInteractionComponent> LeftWidgetInteractionComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Widget Interaction", meta = (AllowPrivateAccess = true))
	TObjectPtr<UWidgetInteractionComponent> RightWidgetInteractionComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Widget Particle", meta = (AllowPrivateAccess = true))
	TObjectPtr<UNiagaraComponent> LeftHandParticle;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Widget Particle", meta = (AllowPrivateAccess = true))
	TObjectPtr<UNiagaraComponent> RightHandParticle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Enhanced Input", meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputConfigData> MovementConfig;
	UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input", meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputMappingContext> MovementContext;




	UFUNCTION()
	void RightTriggerPressed();
	UFUNCTION()
	void LeftTriggerPressed();

	void HandleRightWidget();
	void HandleLeftWidget();


};
