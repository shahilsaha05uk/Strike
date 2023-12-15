// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseInteractable.h"

// Sets default values
void ABaseInteractable::Interact_Implementation(ACharacter* OwnerPlayer)
{
	switch (InteractableItem) {
	case FLAG:
		UE_LOG(LogTemp, Warning, TEXT("Interacted with: Flag"));
		break;
	case WEAPON:
		UE_LOG(LogTemp, Warning, TEXT("Interacted with: Weapon"));
		break;
	}
}

void ABaseInteractable::DropItem_Implementation(ACharacter* OwnerPlayer)
{
}

void ABaseInteractable::AttachToPlayer_Implementation(ACharacter* OwnerPlayer)
{

}

void ABaseInteractable::DetachFromPlayer_Implementation(ACharacter* OwnerPlayer)
{

}

ETeam ABaseInteractable::GetTeam_Implementation()
{
	return Team;
}

EInteractableItem ABaseInteractable::GetInteractableItem_Implementation()
{
	return InteractableItem;
}
