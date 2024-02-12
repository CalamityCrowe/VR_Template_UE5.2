// Fill out your copyright notice in the Description page of Project Settings.


#include "Base_Light.h"

// Sets default values
ABase_Light::ABase_Light()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateOptionalDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

}

// Called when the game starts or when spawned
void ABase_Light::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ABase_Light::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

