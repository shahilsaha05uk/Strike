// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MultiplayerProject/EnumClass.h"
#include "MultiplayerProject/StructClass.h"
#include "UObject/Interface.h"
#include "ControllerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UControllerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MULTIPLAYERPROJECT_API IControllerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void RestartPlayer();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void PawnSetup(UDA_CharacterMeshDetails* CharacterDetails = nullptr);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetPlayerTeam(ETeam Team);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	ETeam GetPlayerTeam();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void IA_RefreshController();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void UpdateScoreboard(int Value, ETeam Team);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void UpdatePlayerHUD(FPlayerDetails PlayerDetails);
};
