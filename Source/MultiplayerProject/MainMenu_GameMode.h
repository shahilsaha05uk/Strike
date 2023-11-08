// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainMenu_GameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerSpawnSignature, class APlayerCharacter*, PlayerRef);

UCLASS()
class MULTIPLAYERPROJECT_API AMainMenu_GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMainMenu_GameMode();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Properties")
	class UMultiplayer_GameInstance* mGameInstance;

	UPROPERTY(BlueprintReadWrite, BlueprintCallable, Category = "Private")
	FOnPlayerSpawnSignature OnPlayerSpawnSignature;


	
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnPlayerSpawn(APlayerCharacter* PlayerRef);
	
};
