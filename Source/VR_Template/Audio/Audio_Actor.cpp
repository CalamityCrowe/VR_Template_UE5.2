// Fill out your copyright notice in the Description page of Project Settings.


#include "Audio_Actor.h"

#include "Components/AudioComponent.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AAudio_Actor::AAudio_Actor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Collider = CreateOptionalDefaultSubobject<UCapsuleComponent>(TEXT("Collider"));
	RootComponent = Collider;

	Mesh = CreateOptionalDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	AudioPlayer = CreateOptionalDefaultSubobject<UAudioComponent>(TEXT("Audio Player"));
	AudioPlayer->SetupAttachment(RootComponent);


	NextActor = nullptr;
	bCollected = false;
	bLast = false;
}

// Called when the game starts or when spawned
void AAudio_Actor::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AAudio_Actor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

