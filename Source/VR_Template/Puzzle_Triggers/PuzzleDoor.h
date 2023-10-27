// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PuzzleDoor.generated.h"

class UTimelineComponent;

UCLASS()
class VR_TEMPLATE_API APuzzleDoor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APuzzleDoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> Mesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<USceneComponent> SceneRoot;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UTimelineComponent> DoorTimeline;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	UCurveFloat* DoorTimeCurve;

	FRotator ClosedRotation;
	FRotator OpenRotation;

	void OpenDoor();




public:
	UStaticMeshComponent* GetMesh() const { return Mesh; }
	USceneComponent* GetSceneRoot() const { return SceneRoot; }



};
