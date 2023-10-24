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
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* ObjectMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector ObjectScale;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector GrabPointScale;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInputMappingContext* ObjectInputMapLeft;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInputMappingContext* ObjectInputMapRight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInputConfigData* ObjectFireInput;




	// needs to be setup like this or the engine will scream at you
	// all the parameters after the constructor are the properties above, these are essentially getting assigned null or the equivalent by default 
	FInteractableData() :
		ObjectType(EObjectType::Light),
		ObjectMesh(nullptr),
		ObjectScale(FVector()),
		GrabPointScale(FVector()),
		ObjectInputMapLeft(nullptr),
		ObjectInputMapRight(nullptr),
		ObjectFireInput(nullptr)
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



private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> m_Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh", meta = (AllowPrivateAccess = true))
	TObjectPtr<UVR_GrabComponent> m_GrabPointSnap;

	// used for tagging the type of object it will be 
	EObjectType m_ObjectTag;


	virtual void FindObjectData(EObjectType);  // this will pull from a data table and then will get any relevant information that will be associated with the object

protected:
	UPROPERTY(EditAnywhere, Category = "Enhanced Input")
	UInputMappingContext* m_InputMappingContextRight;
	UPROPERTY(EditAnywhere, Category = "Enhanced Input")
	UInputMappingContext* m_InputMappingContextLeft;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
	UInputConfigData* m_FireActions; // this is a custom data config that holds a list of pointers that will get assigned in the engine to pull the input actions in

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data Tables")
	TObjectPtr<UDataTable> ObjectData;

	UFUNCTION()
	virtual void BindInteractableInput();
	UFUNCTION()
	virtual void UnbindInput();


	void LoadDataTable(EObjectType);

public:
	inline UStaticMeshComponent* GetMesh() { return m_Mesh; }
	inline UVR_GrabComponent* GetGrabComponent() { return m_GrabPointSnap; }

	inline EObjectType GetObjectType() { return m_ObjectTag; }

};
