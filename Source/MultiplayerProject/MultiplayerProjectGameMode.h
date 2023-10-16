// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "MultiplayerProjectGameMode.generated.h"

UCLASS(minimalapi)
class AMultiplayerProjectGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMultiplayerProjectGameMode();

	class UMultiplayerSubsystem* MultiplayerPlugin;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SessionProperties")
	TEnumAsByte<enum ETypeOfMatch> mMatchType;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Map Reference")
	TSoftObjectPtr<UWorld> mLevelMapRef;

	virtual void BeginPlay() override;

	

	UFUNCTION()
	void OnCreateSession(bool bWasSuccessful);
	void OnFindSessions(const TArray<FOnlineSessionSearchResult>& OnlineSessionSearchResults, bool bWasSuccessful);
	void OnJoinSession(EOnJoinSessionCompleteResult::Type Result);
	UFUNCTION()
	void OnStartSession(bool bBWasSuccessful);
	UFUNCTION()
	void OnDestroySession(bool bBWasSuccessful);

};



