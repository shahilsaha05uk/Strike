// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MultiplayerProjectGameMode.generated.h"

UCLASS(minimalapi)
class AMultiplayerProjectGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMultiplayerProjectGameMode();

	virtual void PostLogin(APlayerController* NewPlayer) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void FindPlayerStarts();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void InitialiseController(int ControllerIndex);
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Initial Properties")
	TArray<AActor*> playerStarts;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Initial Properties")
	TArray<APlayerController*> mControllers;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Initial Properties")
	int mTotalPlayers;

	
	
	
};



