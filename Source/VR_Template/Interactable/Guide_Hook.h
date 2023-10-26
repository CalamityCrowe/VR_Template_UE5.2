// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base_Interactable.h"
#include "Guide_Hook.generated.h"

/**
 *
 */
UCLASS()
class VR_TEMPLATE_API AGuide_Hook : public ABase_Interactable
{
	GENERATED_BODY()
public:
	AGuide_Hook(); 
protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	virtual void Tick(float) override; 
private:
	FVector StartLocation; 
};
