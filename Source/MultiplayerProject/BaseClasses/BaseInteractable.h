// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "MultiplayerProject/StructClass.h"
#include "MultiplayerProject/InterfaceClasses/InteractableInterface.h"
#include "MultiplayerProject/InterfaceClasses/PickupInterface.h"
#include "BaseInteractable.generated.h"

UCLASS()
class MULTIPLAYERPROJECT_API ABaseInteractable : public AActor, public IInteractableInterface
{
	GENERATED_BODY()

public:

	ABaseInteractable();

	
};
