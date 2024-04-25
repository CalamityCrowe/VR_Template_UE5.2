// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "CustomVRLevelScript.generated.h"

/**
 * 
 */
UCLASS()
class CORDELIAS_SIGHT_API ACustomVRLevelScript : public ALevelScriptActor
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


	void AddTimeStamp(float TimeStamp);

	UFUNCTION(BlueprintCallable)
	void PrintTotalTime(); 
private: 
	TArray<float> TimeStamps;

	
	float TotalTime = 0.0f;

	UPROPERTY(EditDefaultsOnly,Category = "Level Reference", meta = (AllowPrivateAccess = true))
	TSoftObjectPtr<UWorld> LevelReference;


};
