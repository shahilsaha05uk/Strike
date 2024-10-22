// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MultiplayerProject/StructClass.h"
#include "UObject/Interface.h"
#include "GameInstanceInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UGameInstanceInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MULTIPLAYERPROJECT_API IGameInstanceInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FMatchDetails GetMatchDetails();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetMatchDetails(FMatchDetails MatchDetails);

	/*
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FPlayerDetails GetPlayerDetails();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetPlayerDetails(FPlayerDetails PlayerDetails);

*/
};
