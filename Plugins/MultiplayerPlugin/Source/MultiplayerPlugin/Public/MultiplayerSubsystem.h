// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "MultiplayerSubsystem.generated.h"

UENUM()
enum ETypeOfMatch
{
	Capture_The_Flag
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStart_OnCreateSession, bool, bWasSuccessful);
DECLARE_MULTICAST_DELEGATE_TwoParams(FStart_OnFindSessions, const TArray<FOnlineSessionSearchResult>& SessionResults, bool bWasSuccessful);
DECLARE_MULTICAST_DELEGATE_OneParam(FStart_OnJoinSession, EOnJoinSessionCompleteResult::Type Result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStart_OnStartSession, bool, bWasSuccessful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStart_OnDestroySession, bool, bWasSuccessful);


UCLASS()
class MULTIPLAYERPLUGIN_API UMultiplayerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

private:

	IOnlineSessionPtr SessionInterface;

	TSharedPtr<FOnlineSessionSettings> NewSessionSettings;
	TSharedPtr<FOnlineSessionSearch> NewSessionSearch;

	// These Delegates are triggered when the task is completed
	FOnCreateSessionCompleteDelegate Completed_CreateSession;
	FDelegateHandle Handle_CreateSessionCompleteDelegate;
	
	FOnFindSessionsCompleteDelegate Completed_FindSessions;
	FDelegateHandle Handle_FindSessionCompleteDelegate;
	
	FOnJoinSessionCompleteDelegate Completed_JoinSession;
	FDelegateHandle Handle_JoinSessionCompleteDelegate;
	
	FOnDestroySessionCompleteDelegate Completed_DestroySession;
	FDelegateHandle Handle_StartSessionCompleteDelegate;
	
	FOnStartSessionCompleteDelegate Completed_StartSession;
	FDelegateHandle Handle_DestroySessionCompleteDelegate;
	
public:

	UMultiplayerSubsystem();

	FStart_OnCreateSession OnCreateSession;
	FStart_OnFindSessions OnFindSessions;
	FStart_OnJoinSession OnJoinSession;
	FStart_OnStartSession OnStartSession;
	FStart_OnDestroySession OnDestroySession;
	
	void CreateSession(int32 NumPublicConnections, TEnumAsByte<ETypeOfMatch> MatchType);
	void FindSessions(int32 MaxSearchResults);
	void JoinSession(const FOnlineSessionSearchResult& SessionResult);
	void StartSession();
	void DestroySession();

	void OnComplete_CreateSession(FName SessionName, bool bSuccess);
	void OnComplete_FindSessions(bool bSuccess);
	void OnComplete_JoinSession(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	void OnComplete_StartSession(FName SessionName, bool bSuccess);
	void OnComplete_DestroySession(FName SessionName, bool bSuccess);

	// Supporting Methods
	static FString GetMatchType(ETypeOfMatch TypeOfMatch);
	
	
};
