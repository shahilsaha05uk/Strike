// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StructClass.h"
#include "Engine/GameInstance.h"

#include "Interfaces/OnlineSessionInterface.h"
#include "Multiplayer_GameInstance.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFindSessionComplete, TArray<FSessionDetails>, SessionDetails);

UCLASS()
class MULTIPLAYERPROJECT_API UMultiplayer_GameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	class UMultiplayerSessionsSubsystem* mMultiplayerSessionsSubsystem;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, BlueprintAssignable, Category = "Private")
	FOnFindSessionComplete OnFindSessionComplete;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Private")
	TSoftObjectPtr<UWorld> mTravelMap;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Private")
	TSoftObjectPtr<UWorld> mMainMenuLevel;

	virtual void Init() override;
	
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
	FString GetLevelPath(TSoftObjectPtr<UWorld> Map, bool shouldListen = true);
	
};

