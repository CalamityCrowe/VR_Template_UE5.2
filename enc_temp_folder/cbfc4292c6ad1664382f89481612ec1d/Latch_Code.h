// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Latch_Code.generated.h"

UCLASS()
class VR_TEMPLATE_API ALatch_Code : public AActor
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
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> m_Mesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<USceneComponent> m_SceneRoot;


	FRotator m_ClosedRotation; 
	FRotator m_OpenRotation; 

protected: 


public: 
	inline UStaticMeshComponent* GetMesh() { return m_Mesh;  }
	inline USceneComponent* GetSceneRoot() { return m_SceneRoot;  }


};
