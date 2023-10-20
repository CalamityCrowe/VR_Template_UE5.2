// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Preasure_Plate.generated.h"

#pragma region forward declerations
class UBoxComponent;
class ALatch_Code;
#pragma endregion 

UCLASS()
class VR_TEMPLATE_API APreasure_Plate : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APreasure_Plate();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnEndOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Collision", meta = (AllowPrivateAccess = true))
	TObjectPtr<UBoxComponent> m_Collider;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<USceneComponent> m_SceneRoot;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Meshes", meta = (AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> m_Mesh;
protected:
	UPROPERTY(EditAnywhere,BlueprintReadOnly, meta = (EditInline), Category = "Object References")
	TSubclassOf<ALatch_Code> m_LatchRefrence;

public:
	inline UBoxComponent* GetCollider() { return m_Collider; }
	inline USceneComponent* GetSceneRoot() { return m_SceneRoot; }
	inline UStaticMeshComponent* GetMesh() { return m_Mesh; }

};
