// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MultiplayerProject/EnumClass.h"
#include "MultiplayerProject/StructClass.h"
#include "UObject/Interface.h"
#include "InteractableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MULTIPLAYERPROJECT_API IInteractableInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	EInteractableItem GetInteractableItem();

	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Interact(ACharacter* OwnerPlayer);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void DropItem(ACharacter* OwnerPlayer);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void AttachToPlayer(ACharacter* OwnerPlayer);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void DetachFromPlayer(ACharacter* OwnerPlayer);
};
