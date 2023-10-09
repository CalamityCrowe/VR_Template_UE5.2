// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Base_Interactable.generated.h"

UCLASS()
class VR_TEMPLATE_API ABase_Interactable : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABase_Interactable();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintImplementableEvent)
		void OnGrabbed();
	UFUNCTION(BlueprintImplementableEvent)
		void OnDropped();
	UFUNCTION()
		void BindInteractableInput();
	UFUNCTION()
		void UnbindInput();


public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mesh")
		TObjectPtr<UStaticMeshComponent> m_Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
		TObjectPtr<class UVR_GrabComponent> m_GrabPointSnap;

	UPROPERTY(EditAnywhere, Category = "Enhanced Input")
		class UInputMappingContext* m_InputMappingContext;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
		class UInputConfigData* m_FireActions; // this is a custom data config that holds a list of pointers that will get assigned in the engine to pull the input actions in



};
