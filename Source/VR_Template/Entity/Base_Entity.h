// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Base_Entity.generated.h"

UENUM(Blueprintable)
enum class EEntityState : uint8
{
	Spawn,
	Idle,
	Chase,
	Dead
};

UCLASS()
class VR_TEMPLATE_API ABase_Entity : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABase_Entity();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Entity", meta = (AllowPrivateAccess = true))
	TEnumAsByte<EEntityState> EntityState;

	UFUNCTION(BlueprintCallable, Category = "Entity")
	void HandleAnimations(); 

private:
	float Health; 

};
