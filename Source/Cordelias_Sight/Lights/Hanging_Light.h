// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base_Light.h"
#include "Hanging_Light.generated.h"


class USpotLightComponent;
/**
 * 
 */
UCLASS()
class CORDELIAS_SIGHT_API AHanging_Light : public ABase_Light
{
	GENERATED_BODY()

public:
	AHanging_Light();

protected: 

	virtual void BeginPlay() override;

public:

	virtual void Tick(float DeltaSeconds) override;

private:

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Light",meta = (AllowPrivateAccess = true))
	TObjectPtr<USpotLightComponent> SpotLight; 


};
