// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StructClass.h"
#include "Engine/GameInstance.h"
#include "InterfaceClasses/GameInstanceInterface.h"

#include "Interfaces/OnlineSessionInterface.h"
#include "Multiplayer_GameInstance.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFindSessionComplete, TArray<FSessionDetails>, SessionDetails);

UCLASS()
class MULTIPLAYERPROJECT_API UMultiplayer_GameInstance : public UGameInstance, public IGameInstanceInterface
{
	GENERATED_BODY()

public:
	class ULAN_OnlineSubsystem* mMultiplayerSessionsSubsystem;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, BlueprintAssignable, Category = "Private")
	FOnFindSessionComplete OnFindSessionComplete;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Private")
	TSoftObjectPtr<UWorld> mTravelMap;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Private")
	TSoftObjectPtr<UWorld> mMainMenuLevel;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Private")
	FMatchDetails mMatchDetails;
	
	virtual void Init() override;
	virtual FMatchDetails GetMatchDetails_Implementation() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetSessionDetails(FMatchDetails MatchDetails);
	
	UFUNCTION()
	void OnCreateSession(bool bWasSuccessful);
	void OnFindSessions(const TArray<FSessionDetails>& OnlineSessionSearchResults, bool bWasSuccessful);
	void OnJoinSession(EOnJoinSessionCompleteResult::Type Result);
	UFUNCTION()
	void OnStartSession(bool bBWasSuccessful);
	UFUNCTION()
	void OnDestroySession(bool bWasSuccessful);


public:

	UFUNCTION(BlueprintCallable)
	FString GetLevelPath(TSoftObjectPtr<UWorld> Map, bool shouldListen = false);
	
};

