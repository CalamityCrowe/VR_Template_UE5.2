// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "Mannequin_Hands.generated.h"

/**
 * 
 */
UCLASS()
class VR_TEMPLATE_API UMannequin_Hands : public USkeletalMeshComponent
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		bool mb_isMirrored; 

};
