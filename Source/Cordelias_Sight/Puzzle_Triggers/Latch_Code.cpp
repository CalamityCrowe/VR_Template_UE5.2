// Fill out your copyright notice in the Description page of Project Settings.


#include "Latch_Code.h"

// Sets default values
ALatch_Code::ALatch_Code()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateOptionalDefaultSubobject<USceneComponent>(TEXT("Scene Root"));
	RootComponent = SceneRoot;

	auto MeshAsset = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Plane.Shape_Plane'"));
	Mesh = CreateOptionalDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(SceneRoot);
	Mesh->SetStaticMesh(MeshAsset.Object);

	m_isOpen = false;


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

