// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "CustomVRLevelScript.generated.h"

/**
 * 
 */
UCLASS()
class VR_TEMPLATE_API ACustomVRLevelScript : public ALevelScriptActor
{
	GENERATED_BODY()
	
public:
	ACustomVRLevelScript();

protected:
	virtual void BeginPlay() override;
public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent)
	void LevelFinished(); 

};
