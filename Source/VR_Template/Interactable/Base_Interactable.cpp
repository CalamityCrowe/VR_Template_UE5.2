// Fill out your copyright notice in the Description page of Project Settings.


#include "Base_Interactable.h"
#include "VR_Template/VR_Components/VR_GrabComponent.h"
#include "Kismet/GameplayStatics.h"

#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"

#include "Haptics/HapticFeedbackEffect_Curve.h"

// Sets default values
ABase_Interactable::ABase_Interactable()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateOptionalDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	GrabPoint = CreateOptionalDefaultSubobject<UVR_GrabComponent>(TEXT("Grab Point"));
	GrabPoint->SetupAttachment(Mesh);
	GrabPoint->SetGrabType(GrabTypes::Free);


	///
	//

}

void ABase_Interactable::LoadDataTable(EObjectType newObj)
{
	auto DataTable = ConstructorHelpers::FObjectFinder<UDataTable>(TEXT("DataTable'/Game/Data_Tables/MDT_Object_Data.MDT_Object_Data'"));
	ObjectData = DataTable.Object;
	if (ObjectData)
	{
		FindObjectData(newObj);
	}
}

void ABase_Interactable::LoadHapticEffects(EObjectType newObj)
{
	auto DataTable = ConstructorHelpers::FObjectFinder<UDataTable>(TEXT("DataTable'/Game/Data_Tables/MDT_Haptic_Data.MDT_Haptic_Data'"));
	ObjectData = DataTable.Object;
	if (ObjectData)
	{
		FindHapticData(newObj);
	}
}

// Called when the game starts or when spawned
void ABase_Interactable::BeginPlay()
{
	Super::BeginPlay();

	if (GrabPoint)
	{
		GrabPoint->OnGrabbedDelegate.AddUObject(this, &ABase_Interactable::BindInteractableInput);
		GrabPoint->OnDroppedDelegate.AddUObject(this, &ABase_Interactable::UnbindInput); // this is used for binding inputs within interactable objects
	}

}

// Called every frame
void ABase_Interactable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



void ABase_Interactable::FindObjectData(EObjectType newObj)
{
	EObjectType FindThisType = newObj;

	FInteractableData* FoundRow = nullptr;

	for (auto It : ObjectData->GetRowMap())
	{
		if (FInteractableData* Row = reinterpret_cast<FInteractableData*>(It.Value))
		{
			if (Row->ObjectType == FindThisType)
			{
				FoundRow = Row;
				break;
			}
		}
	}
	if (FoundRow != nullptr)
	{

		UE_LOG(LogTemp, Warning, TEXT("Found Row"));
		m_ObjectTag = FoundRow->ObjectType;
		Mesh->SetStaticMesh(FoundRow->ObjectMesh);
		SetActorScale3D(FoundRow->ObjectScale);
		GrabPoint->SetRelativeScale3D(FoundRow->GrabPointScale);

		switch (m_ObjectTag)
		{
		case EObjectType::None:
			GrabPoint->SetGrabType(GrabTypes::None);
			break;

		case EObjectType::Gun:
		case EObjectType::Light:
			GrabPoint->SetGrabType(GrabTypes::Snap);
			break;
		case EObjectType::Throwable:
		case EObjectType::Moveable:
			GrabPoint->SetGrabType(GrabTypes::Free);
			break;
		}
		InputMappingContextLeft = FoundRow->ObjectInputMapLeft;
		InputMappingContextRight = FoundRow->ObjectInputMapRight;
		m_FireActions = FoundRow->ObjectFireInput;
	}
}

void ABase_Interactable::FindHapticData(EObjectType newObj)
{
	EObjectType FindThisType = newObj;

	FHapticEffects* FoundRow = nullptr;

	for (auto It : ObjectData->GetRowMap())
	{
		if (FHapticEffects* Row = reinterpret_cast<FHapticEffects*>(It.Value))
		{
			if (Row->ObjectType == FindThisType)
			{
				FoundRow = Row;
				break;
			}
		}
	}
	if (FoundRow != nullptr)
	{

		GrabPoint->SetGrabHapticEffect(FoundRow->ObjectGrabFeedback);
		GrabPoint->SetInteractHapticEffect(FoundRow->ObjectInteractFeedback);
	}
}

void ABase_Interactable::BindInteractableInput()
{

	if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		EnableInput(PC);
		if (ULocalPlayer* LocalPlayer = PC->GetLocalPlayer())
		{
			if (UEnhancedInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
			{
				switch (GrabPoint->GetHeldByHand())
				{
				case EControllerHand::Left:
					UE_LOG(LogTemp, Warning, TEXT("Left Hand Mapping Context Bound"));
					InputSystem->AddMappingContext(InputMappingContextLeft, 0);
					break;
				case EControllerHand::Right:
					UE_LOG(LogTemp, Warning, TEXT("Right Hand Mapping Context Bound"));
					InputSystem->AddMappingContext(InputMappingContextRight, 0);
					break;
				default:
					break;
				}
			}
		}
	}


}

void ABase_Interactable::UnbindInput()
{

	GEngine->AddOnScreenDebugMessage(0, 2.f, FColor::Red, TEXT("Detached Component, Base Interactable.cpp"));
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		DisableInput(PC);
		if (const ULocalPlayer* LocalPlayer = PC->GetLocalPlayer())
		{
			if (UEnhancedInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
			{
				switch (GrabPoint->GetHeldByHand())
				{
				case EControllerHand::Left:
					InputSystem->RemoveMappingContext(InputMappingContextLeft);
					break;
				case EControllerHand::Right:
					InputSystem->RemoveMappingContext(InputMappingContextRight);
					break;
				default:

					break;
				}
			}
		}
	}

}

