// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "Base_Interactable.generated.h"

class UVR_GrabComponent;
class UInputMappingContext;
class UInputConfigData;
class UHapticFeedbackEffect_Curve;

UENUM()
enum class EObjectType : uint8
{
	None,
	Gun,
	Light,
	Throwable,
	Moveable

};



USTRUCT(BlueprintType)
struct VR_TEMPLATE_API FInteractableData : public FTableRowBase
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EObjectType ObjectType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMesh> ObjectMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector ObjectScale;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector GrabPointScale;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UInputMappingContext> ObjectInputMapLeft;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UInputMappingContext> ObjectInputMapRight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UInputConfigData> ObjectFireInput;




	// needs to be setup like this or the engine will scream at you
	// all the parameters after the constructor are the properties above, these are essentially getting assigned null or the equivalent by default 
	FInteractableData() :
		ObjectType(EObjectType::None),
		ObjectMesh(nullptr),
		ObjectScale(FVector()),
		GrabPointScale(FVector()),
		ObjectInputMapLeft(nullptr),
		ObjectInputMapRight(nullptr),
		ObjectFireInput(nullptr)
	{

	}
};

USTRUCT(BlueprintType)
struct FHapticEffects :public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EObjectType ObjectType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UHapticFeedbackEffect_Curve* ObjectInteractFeedback;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UHapticFeedbackEffect_Curve* ObjectGrabFeedback;

	FHapticEffects() :
		ObjectType(EObjectType::None),
		ObjectInteractFeedback(nullptr),
		ObjectGrabFeedback(nullptr)
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
	TObjectPtr<UStaticMeshComponent> Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh", meta = (AllowPrivateAccess = true))
	TObjectPtr<UVR_GrabComponent> GrabPoint;

	// used for tagging the type of object it will be 
	EObjectType m_ObjectTag;


	void FindObjectData(EObjectType);  // this will pull from a data table and then will get any relevant information that will be associated with the object
	void FindHapticData(EObjectType);

protected:
	UPROPERTY(EditAnywhere, Category = "Enhanced Input")
	UInputMappingContext* InputMappingContextRight;
	UPROPERTY(EditAnywhere, Category = "Enhanced Input")
	UInputMappingContext* InputMappingContextLeft;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
	UInputConfigData* m_FireActions; // this is a custom data config that holds a list of pointers that will get assigned in the engine to pull the input actions in

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data Tables")
	TObjectPtr<UDataTable> ObjectData;

	UFUNCTION()
	virtual void BindInteractableInput();
	UFUNCTION()
	virtual void UnbindInput();


	void LoadDataTable(EObjectType);
	void LoadHapticEffects(EObjectType);

public:
	inline UStaticMeshComponent* GetMesh() const { return Mesh; }
	inline UVR_GrabComponent* GetGrabComponent() const { return GrabPoint; }

	inline EObjectType GetObjectType() const { return m_ObjectTag; }

};
