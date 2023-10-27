// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainMenu_GameMode.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERPROJECT_API AMainMenu_GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Properties")
	class UMultiplayer_GameInstance* mGameInstance;
};
