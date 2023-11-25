// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MultiplayerProject/EnumClass.h"
#include "UObject/Interface.h"
#include "FlagInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UFlagInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MULTIPLAYERPROJECT_API IFlagInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnDropped();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnPickedUp(APlayerState* PlayerState);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	ETeam GetFlagTeam();

	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ResetFlag();
};
