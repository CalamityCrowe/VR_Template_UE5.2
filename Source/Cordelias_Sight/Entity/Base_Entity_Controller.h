// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Base_Entity_Controller.generated.h"

class ABase_Entity;

/**
 *
 */
UCLASS()
class CORDELIAS_SIGHT_API ABase_Entity_Controller : public AAIController
{
	GENERATED_BODY()
public: // for the constructors
	ABase_Entity_Controller();
protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void OnPossess(APawn* InPawn) override;
public: // for the functions
	void MoveActor(AActor*);



protected: // for variables

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	TObjectPtr<AActor> TargetActorReference;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI")
	TObjectPtr<ABase_Entity> ControlledPawnReference; // for the pawn reference


};
