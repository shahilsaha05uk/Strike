// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MultiplayerProject/EnumClass.h"
#include "MultiplayerProject/StructClass.h"
#include "UObject/Interface.h"
#include "PlayerHUDInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPlayerHUDInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MULTIPLAYERPROJECT_API IPlayerHUDInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void UpdateHUD(FPlayerDetails PlayerDetails);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void UpdateScore(int Score, ETeam Team);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void UpdateHealth(float Value);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void UpdateMoney(int Value);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void UpdateAmmo(int Value);
	

};
