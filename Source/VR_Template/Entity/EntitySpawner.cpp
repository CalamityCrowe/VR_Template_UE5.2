// Fill out your copyright notice in the Description page of Project Settings.


#include "EntitySpawner.h"

#include "Base_Entity.h"
#include "Base_Entity_Controller.h"

// Sets default values
AEntitySpawner::AEntitySpawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bCanSpawn = true;

}

// Called when the game starts or when spawned
void AEntitySpawner::BeginPlay()
{
	Super::BeginPlay();

}
// Called every frame
void AEntitySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEntitySpawner::SpawnEnemy()
{
	if (bCanSpawn)
	{
		const FActorSpawnParameters SpawnParameters;

		if (ABase_Entity_Controller* NewController = GetWorld()->SpawnActor<ABase_Entity_Controller>(ABase_Entity_Controller::StaticClass()))
		{
			if (ABase_Entity* newEntity = GetWorld()->SpawnActor<ABase_Entity>(ABase_Entity::StaticClass()))
			{
				NewController->Possess(newEntity);
				newEntity->AIControllerClass = ABase_Entity_Controller::StaticClass();
				newEntity->Controller = NewController;
			}
		}
		bCanSpawn = false;
	}

}



