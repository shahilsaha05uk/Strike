// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MultiplayerProject/InterfaceClasses/InteractableInterface.h"

// Sets default values for this component's properties
UInteractableComponent::UInteractableComponent()
{

}

void UInteractableComponent::Interact_Implementation(AActor* TargetActor, ACharacter* OwningCharacter)
{
	if(UKismetSystemLibrary::DoesImplementInterface(TargetActor, UInteractableInterface::StaticClass()))
	{
		IInteractableInterface::Execute_Interact(TargetActor, OwningCharacter);
	}
}

void UInteractableComponent::DropItem_Implementation(AActor* TargetActor, ACharacter* OwningCharacter)
{
	if(UKismetSystemLibrary::DoesImplementInterface(TargetActor, UInteractableInterface::StaticClass()))
	{
		IInteractableInterface::Execute_DropItem(TargetActor, OwningCharacter);
	}
}
