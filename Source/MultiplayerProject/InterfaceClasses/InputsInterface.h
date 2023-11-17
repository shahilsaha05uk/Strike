// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MultiplayerProject/StructClass.h"
#include "UObject/Interface.h"
#include "InputsInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable, BlueprintType)
class UInputsInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MULTIPLAYERPROJECT_API IInputsInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void InitHUD(FPlayerDetails PlayerDetails);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void UpdateOverlayUI();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void UpdateFocusedActor(FInteractableDetails Details);

};
