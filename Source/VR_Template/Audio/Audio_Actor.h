// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Audio_Actor.generated.h"

class USoundCue;
class UCapsuleComponent;
class USplineComponent;
class ASplineActor;
class UNiagaraComponent; 

UCLASS()
class VR_TEMPLATE_API AAudio_Actor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAudio_Actor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> Mesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Collider", meta = (AllowPrivateAccess = true))
	TObjectPtr<UCapsuleComponent>Collider;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio", meta = (AllowPrivateAccess = true))
	TObjectPtr<UAudioComponent> AudioPlayer;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Actor Reference", meta = (AllowPrivateAccess = true))
	TObjectPtr<AAudio_Actor> NextActor;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Actor Reference", meta = (AllowPrivateAccess = true))
	TObjectPtr<ASplineActor> SplineReference;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Niagara",meta = (AllowPrivateAccess = true))
	TObjectPtr<UNiagaraComponent> NiagaraComponent;


	UPROPERTY(EditAnywhere, Category = "bools", meta = (AllowPrivateAccess = true));
	bool bActive;
	UPROPERTY(EditAnywhere, Category = "bools", meta = (AllowPrivateAccess = true));
	bool bLast;
	UPROPERTY(EditAnywhere, Category = "bools", meta = (AllowPrivateAccess = true));
	bool bMoveable;
	UPROPERTY(EditAnywhere, Category = "Speed", meta = (AllowPrivateAccess = true));
	float InterpSpeed;


	float Time; 

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	void ActivateActor();
};
