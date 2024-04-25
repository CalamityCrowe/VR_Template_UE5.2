// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EntitySpawner.generated.h"

UCLASS()
class CORDELIAS_SIGHT_API AEntitySpawner : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEntitySpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawner", meta = (AllowPrivateAccess = true))
	TSubclassOf<class ABase_Entity> EntityToSpawn;

	bool bCanSpawn;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UFUNCTION(BlueprintCallable, Category = "Spawner")
	void SpawnEnemy(); 

	

};
