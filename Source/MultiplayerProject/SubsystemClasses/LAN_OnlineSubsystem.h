// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "MultiplayerProject/StructClass.h"
#include "LAN_OnlineSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMultiplayerOnCreateSessionComplete, bool, bWasSuccessful);
DECLARE_MULTICAST_DELEGATE_TwoParams(FMultiplayerOnFindSessionsComplete, const TArray<FSessionDetails>& SessionResults, bool bWasSuccessful);
DECLARE_MULTICAST_DELEGATE_OneParam(FMultiplayerOnJoinSessionComplete, EOnJoinSessionCompleteResult::Type Result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMultiplayerOnStartSessionComplete, bool, bWasSuccessful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMultiplayerOnDestroySessionComplete, bool, bWasSuccessful);

UCLASS()
class MULTIPLAYERPROJECT_API ULAN_OnlineSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	ULAN_OnlineSubsystem();
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SessionProperties")
	FMatchDetails mMatchDetails;
	
	IOnlineSessionPtr mSessionInterface;

	TSharedPtr<class FOnlineSessionSettings> mLastSessionSettings;

	TSharedPtr<FOnlineSessionSearch> mLastSessionSearch;

	/*
	 * To handle session functionality, the menu will call these
	*/

	UFUNCTION(BlueprintCallable)
	void CreateSession(int32 NumPublicConnections, FString MatchType);

	UFUNCTION(BlueprintCallable)
	void FindSessions(int32 MaxSearchResults);

	UFUNCTION(BlueprintCallable)
	void JoinSession(FSessionDetails SessionResult);

	UFUNCTION(BlueprintCallable)
	void StartSession();

	UFUNCTION(BlueprintCallable)
	void DestroySession();

	// Custom Delegates for communicating with the Menu
	FMultiplayerOnCreateSessionComplete mMultiplayerOnCreateSessionComplete;

	FMultiplayerOnFindSessionsComplete mMultiplayerOnFindSessionsComplete;

	FMultiplayerOnJoinSessionComplete mMultiplayerOnJoinSessionComplete;

	FMultiplayerOnStartSessionComplete mMultiplayerOnStartSessionComplete;

	FMultiplayerOnDestroySessionComplete mMultiplayerOnDestroySessionComplete;
protected:

	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
	
	void OnFindSessionsComplete(bool bWasSuccessful);
	
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	
	void OnStartSessionComplete(FName SessionName, bool bWasSuccessful);
	
	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);
	
private:

	/*
	* Delegates to bind to the methods
	*/

	FOnCreateSessionCompleteDelegate mOnCreateSessionCompleteDelegate;
	FDelegateHandle mCreateSessionCompleteDelegateHandle;
	
	FOnFindSessionsCompleteDelegate mOnFindSessionsCompleteDelegate;
	FDelegateHandle mFindSessionsCompleteDelegateHandle;
	
	FOnJoinSessionCompleteDelegate mOnJoinSessionCompleteDelegate;
	FDelegateHandle mJoinSessionCompleteDelegateHandle;
	
	FOnStartSessionCompleteDelegate mOnStartSessionCompleteDelegate;
	FDelegateHandle mStartSessionCompleteDelegateHandle;
	
	FOnDestroySessionCompleteDelegate mOnDestroySessionCompleteDelegate;
	FDelegateHandle mDestroySessionCompleteDelegateHandle;

	
	bool TryTravelToCurrentSession();
};
