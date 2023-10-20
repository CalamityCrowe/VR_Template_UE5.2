// Fill out your copyright notice in the Description page of Project Settings.


#include "Preasure_Plate.h"
#include "Components/BoxComponent.h"


// Sets default values
APreasure_Plate::APreasure_Plate()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	m_SceneRoot = CreateOptionalDefaultSubobject<USceneComponent>(TEXT("Scene Root"));
	RootComponent = m_SceneRoot;

	m_Collider = CreateOptionalDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	m_Collider->SetupAttachment(m_SceneRoot);
	m_Collider->SetBoxExtent(FVector(50, 50, 50));
	m_Collider->ShapeColor = FColor(0, 255, 128);

	m_Mesh = CreateOptionalDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	m_Mesh->SetupAttachment(m_SceneRoot);

}

// Called when the game starts or when spawned
void APreasure_Plate::BeginPlay()
{
	Super::BeginPlay();
	m_Collider->OnComponentBeginOverlap.AddDynamic(this, &APreasure_Plate::OnBeginOverlap);
	m_Collider->OnComponentEndOverlap.AddDynamic(this, &APreasure_Plate::OnEndOverlap);
}

void APreasure_Plate::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void APreasure_Plate::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

// Called every frame
void APreasure_Plate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

