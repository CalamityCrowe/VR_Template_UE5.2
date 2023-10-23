// Fill out your copyright notice in the Description page of Project Settings.


#include "Base_Interactable.h"
#include "VR_Template/VR_Components/VR_GrabComponent.h"
#include "Kismet/GameplayStatics.h"

#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
// Sets default values
ABase_Interactable::ABase_Interactable()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_Mesh = CreateOptionalDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = m_Mesh;

	m_GrabPointSnap = CreateOptionalDefaultSubobject<UVR_GrabComponent>(TEXT("Grab Point"));
	m_GrabPointSnap->SetupAttachment(m_Mesh);
	if (m_GrabPointSnap->GetGrabType() == GrabType::None) // checks if there is no grab type set and if so sets it to snap 
	{
		m_GrabPointSnap->SetGrabType(GrabType::Snap);
	}

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

// Called when the game starts or when spawned
void ABase_Interactable::BeginPlay()
{
	Super::BeginPlay();

	if (m_GrabPointSnap)
	{
		m_GrabPointSnap->m_OnGrabbed.BindUObject(this, &ABase_Interactable::BindInteractableInput);
		m_GrabPointSnap->m_OnDropped.BindUObject(this, &ABase_Interactable::UnbindInput); // this is used for binding inputs within interactable objects
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
		FInteractableData* Row = reinterpret_cast<FInteractableData*>(It.Value);
		if (Row->ObjectType == FindThisType)
		{
			FoundRow = Row;
			break;
		}
	}
	if (FoundRow != nullptr)
	{

		m_ObjectTag = FoundRow->ObjectType;
		m_Mesh->SetStaticMesh(FoundRow->ObjectMesh);
		SetActorScale3D(FoundRow->ObjectScale);
		m_GrabPointSnap->SetRelativeScale3D(FoundRow->GrabPointScale);

		switch (m_ObjectTag)
		{
		case EObjectType::None:
			m_GrabPointSnap->SetGrabType(GrabType::None);
			break;

		case EObjectType::Gun:
		case EObjectType::Light:
			m_GrabPointSnap->SetGrabType(GrabType::Snap);
			break;
		case EObjectType::Throwable:
			m_GrabPointSnap->SetGrabType(GrabType::Free);
			break;
		}
		m_InputMappingContext = FoundRow->ObjectInputMap;
		m_FireActions = FoundRow->ObjectFireInput;
	}
}

void ABase_Interactable::BindInteractableInput()
{
	GEngine->AddOnScreenDebugMessage(0, 2.f, FColor::Red, TEXT("Attached Component, Base Interactable.cpp"));

	if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		EnableInput(PC);
		if (ULocalPlayer* LocalPlayer = PC->GetLocalPlayer())
		{
			if (UEnhancedInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
			{
				switch (m_GrabPointSnap->GetHeldByHand())
				{
				case EControllerHand::Left:
					InputSystem->AddMappingContext(m_InputMappingContext, 1);
					break;
				case EControllerHand::Right:
					InputSystem->AddMappingContext(m_InputMappingContext, 1);
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
		if (ULocalPlayer* LocalPlayer = PC->GetLocalPlayer())
		{
			if (UEnhancedInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
			{
				switch (m_GrabPointSnap->GetHeldByHand())
				{
				case EControllerHand::Left:
					InputSystem->RemoveMappingContext(m_InputMappingContext);
					break;
				case EControllerHand::Right:
					InputSystem->RemoveMappingContext(m_InputMappingContext);
					break;
				}
			}
		}
	}

}

