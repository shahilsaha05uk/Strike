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

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Map Reference")
	TSoftObjectPtr<UWorld> mLevelMapRef;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void UpdateOnNewPlayerJoin();

	virtual void BeginPlay() override;
};



