// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base_Interactable.h"
#include "Interactable_Stungun.generated.h"

/**
 *
 */
UCLASS()
class VR_TEMPLATE_API AInteractable_Stungun : public ABase_Interactable
{
	GENERATED_BODY()
public:
	AInteractable_Stungun();
	virtual ~AInteractable_Stungun() = default;



protected:

	virtual void BeginPlay() override;

	void FireStunGun();

private:

	bool LineTraceMethod(FHitResult& OutHit,const FVector& StartLocation, const FVector& EndLocation);
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Arrow", meta = (AllowPrivateAccess = true))
	TObjectPtr<class UArrowComponent> FireDirection;

};
