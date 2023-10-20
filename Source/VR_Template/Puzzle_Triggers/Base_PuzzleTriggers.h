// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Base_PuzzleTriggers.generated.h"

UCLASS()
class VR_TEMPLATE_API ABase_PuzzleTriggers : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABase_PuzzleTriggers();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Collision", meta = (AllowPrivateAccess = true))
	TObjectPtr<class UBoxComponent> m_Collider;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<USceneComponent> m_SceneRoot; 


public:
	inline UBoxComponent* GetCollider() { return m_Collider; }
	inline USceneComponent* GetSceneRoot() { return m_SceneRoot;  }
};
