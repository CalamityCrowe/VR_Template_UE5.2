// Fill out your copyright notice in the Description page of Project Settings.


#include "Latch_Code.h"

// Sets default values
ALatch_Code::ALatch_Code()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_SceneRoot = CreateOptionalDefaultSubobject<USceneComponent>(TEXT("Scene Root"));
	RootComponent = m_SceneRoot;

	m_Mesh = CreateOptionalDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh")); 
	m_Mesh->SetupAttachment(m_SceneRoot); 

}

// Called when the game starts or when spawned
void ALatch_Code::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ALatch_Code::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

