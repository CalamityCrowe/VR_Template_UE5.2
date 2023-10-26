// Fill out your copyright notice in the Description page of Project Settings.


#include "Pressure_Plate.h"
#include "Components/BoxComponent.h"
#include "Latch_Code.h"


// Sets default values
APressure_Plate::APressure_Plate()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SceneRoot = CreateOptionalDefaultSubobject<USceneComponent>(TEXT("Scene Root"));
	RootComponent = SceneRoot;

	Collider = CreateOptionalDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	Collider->SetupAttachment(SceneRoot);
	Collider->SetBoxExtent(FVector(50, 50, 50));
	Collider->ShapeColor = FColor(0, 255, 128);

	Mesh = CreateOptionalDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(SceneRoot);

}

// Called when the game starts or when spawned
void APressure_Plate::BeginPlay()
{
	Super::BeginPlay();
	Collider->OnComponentBeginOverlap.AddDynamic(this, &APressure_Plate::OnBeginOverlap);
	Collider->OnComponentEndOverlap.AddDynamic(this, &APressure_Plate::OnEndOverlap);
}

void APressure_Plate::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsValid(LatchReference) && LatchReference->IsA(ALatch_Code::StaticClass()))
	{
		LatchReference->ChangeLatchRotation();
	}
}

void APressure_Plate::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (IsValid(LatchReference) && LatchReference->IsA(ALatch_Code::StaticClass()))
	{
		LatchReference->ChangeLatchRotation();
	}
}

// Called every frame
void APressure_Plate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

