// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BaseVrGamemode.generated.h"

/**
 * 
 */
UCLASS()
class CORDELIAS_SIGHT_API ABaseVrGamemode : public AGameModeBase
{
	GENERATED_BODY()
public:
	ABaseVrGamemode(); 
	virtual ~ABaseVrGamemode() = default; 
public: 
	virtual void StartPlay() override; 

protected:
	virtual void Tick(float) override; 

};
