// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FileReader.generated.h"

/**
 * 
 */
UCLASS()
class VR_TEMPLATE_API UFileReader : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()


public:
	UFUNCTION(BlueprintPure, meta = (Keywords = "Load Text"))
	static bool LoadText(FString FileNameA, FString& SaveTextA);

	UFUNCTION(BlueprintPure, meta = (Keywords = "Save Text"))
	static bool SaveText(FString SaveTextB, FString FileNameB);

};
