// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pressure_Plate.generated.h"

#pragma region forward declerations
class UBoxComponent;
class ALatch_Code;
#pragma endregion 

UCLASS()
class CORDELIAS_SIGHT_API APressure_Plate : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APressure_Plate();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnEndOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Object References")
	TObjectPtr<ALatch_Code> LatchReference;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Collision", meta = (AllowPrivateAccess = true))
	TObjectPtr<UBoxComponent> Collider;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<USceneComponent> SceneRoot;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Meshes", meta = (AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> Mesh;


public:
	inline UBoxComponent* GetCollider() const { return Collider; }
	inline USceneComponent* GetSceneRoot() const { return SceneRoot; }
	inline UStaticMeshComponent* GetMesh() const { return Mesh; }

};
