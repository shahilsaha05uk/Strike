// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "MultiplayerProject/StructClass.h"
#include "UObject/Interface.h"
#include "PlayerInputInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPlayerInputInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MULTIPLAYERPROJECT_API IPlayerInputInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Move(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Look(const FInputActionValue& Value);

	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Jumping(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void StopJump();

	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void StartAiming();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void StopAiming();

	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void StartShooting();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void StopShooting();

	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Interact();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void DropItem();
};
