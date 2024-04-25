// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "BaseVRGameState.generated.h"

/**
 * 
 */
UCLASS()
class CORDELIAS_SIGHT_API ABaseVRGameState : public AGameStateBase
{
	GENERATED_BODY()
public:
	ABaseVRGameState(); 
	virtual ~ABaseVRGameState() = default; 

};
