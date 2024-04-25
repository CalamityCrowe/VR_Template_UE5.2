// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Latch_Code.generated.h"

UCLASS()
class CORDELIAS_SIGHT_API ALatch_Code : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALatch_Code();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;



	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool m_isOpen;


private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> Mesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<USceneComponent> SceneRoot;

public:
	inline UStaticMeshComponent* GetMesh() const { return Mesh; }
	inline USceneComponent* GetSceneRoot() const { return SceneRoot; }

	UFUNCTION(BlueprintImplementableEvent)
	void ChangeLatchRotation();

};
