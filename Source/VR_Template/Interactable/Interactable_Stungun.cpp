// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable_Stungun.h"

AInteractable_Stungun::AInteractable_Stungun()
{
	auto MeshAsset = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Game/Models/Glock-17.Glock-17'"));
	GetMesh()->SetStaticMesh(MeshAsset.Object);
}



void AInteractable_Stungun::FireStunGun()
{

}
