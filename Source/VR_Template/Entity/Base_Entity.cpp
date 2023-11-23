// Fill out your copyright notice in the Description page of Project Settings.


#include "Base_Entity.h"

// Sets default values
ABase_Entity::ABase_Entity()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	auto LoadedMesh = ConstructorHelpers::FObjectFinder<USkeletalMesh>(TEXT("SkeletalMesh'/Game/UndeadPack/Lich/Mesh/SK_Lich.SK_Lich'"));
	GetMesh()->SetSkeletalMesh(LoadedMesh.Object);
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	auto AnimationBP = ConstructorHelpers::FObjectFinder<UAnimBlueprint>(TEXT("AnimBlueprint'/Game/UndeadPack/Lich/Animations/ABP_Lich.ABP_Lich'"));
	GetMesh()->SetAnimInstanceClass(AnimationBP.Object->GeneratedClass);
	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f));

	GetMesh()->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));

	EntityState = EEntityState::Idle;
	Health = 100.0f;
}

// Called when the game starts or when spawned
void ABase_Entity::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ABase_Entity::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABase_Entity::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABase_Entity::HandleAnimations()
{
	FVector EntityVelocity = GetVelocity();
	float VelocitySqr = EntityVelocity.SizeSquared();

	EntityState = (VelocitySqr > 0) ? EEntityState::Chase : EEntityState::Idle;
	EntityState = (Health <= 0) ? EEntityState::Dead : EntityState;
}

