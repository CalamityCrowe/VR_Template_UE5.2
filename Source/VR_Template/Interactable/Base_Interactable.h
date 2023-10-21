// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "Base_Interactable.generated.h"

class UVR_GrabComponent;
class UInputMappingContext;
class UInputConfigData;

UENUM()
enum class EObjectType : uint8
{
	None,
	Gun,
	Light,
	Throwable,

};


USTRUCT(BlueprintType)
struct VR_TEMPLATE_API FInteractableData : public FTableRowBase
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EObjectType ObjectType;

	// needs to be setup like this or the engine will scream at you
	FInteractableData() :ObjectType(EObjectType::None)
	{

	}
};

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




private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> m_Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh", meta = (AllowPrivateAccess = true))
	TObjectPtr<UVR_GrabComponent> m_GrabPointSnap;

	// used for tagging the type of object it will be 
	EObjectType m_ObjectTag;

	UFUNCTION()
	void BindInteractableInput();
	UFUNCTION()
	void UnbindInput();

	virtual void FindObjectData(EObjectType);  // this will pull from a data table and then will get any relevant information that will be associated with the object
protected:
	UPROPERTY(EditAnywhere, Category = "Enhanced Input")
	UInputMappingContext* m_InputMappingContext;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
	UInputConfigData* m_FireActions; // this is a custom data config that holds a list of pointers that will get assigned in the engine to pull the input actions in

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data Tables")
	TObjectPtr<UDataTable> ObjectData;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data Tables")
	EObjectType ObjectTypeReference;
	

public:
	UStaticMeshComponent* GetMesh() { return m_Mesh; }
	UVR_GrabComponent* GetGrabComponent() { return m_GrabPointSnap; }

};
