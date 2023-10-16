// Copyright Epic Games, Inc. All Rights Reserved.

#include "MultiplayerProjectGameMode.h"
#include "MultiplayerSubsystem.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"


AMultiplayerProjectGameMode::AMultiplayerProjectGameMode()
{

}

void AMultiplayerProjectGameMode::BeginPlay()
{
	if(UGameInstance* GameInstance = GetGameInstance())
	{
		MultiplayerPlugin = GameInstance->GetSubsystem<UMultiplayerSubsystem>();
	}

	if(MultiplayerPlugin)
	{
		MultiplayerPlugin->OnStartSession.AddDynamic(this, &ThisClass::OnCreateSession);
		MultiplayerPlugin->OnFindSessions.AddUObject(this,  &ThisClass::OnFindSessions);
		MultiplayerPlugin->OnJoinSession.AddUObject(this,  &ThisClass::OnJoinSession);
		MultiplayerPlugin->OnStartSession.AddDynamic(this, &ThisClass::OnStartSession);
		MultiplayerPlugin->OnDestroySession.AddDynamic(this, &ThisClass::OnDestroySession);
	}


	Super::BeginPlay();
}

void AMultiplayerProjectGameMode::OnCreateSession(bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		UWorld* World = GetWorld();

		if(World)
		{
			World->ServerTravel(FString("/Game/Maps/Level1?listen"));
		}
	}
}

void AMultiplayerProjectGameMode::OnFindSessions(const TArray<FOnlineSessionSearchResult>& OnlineSessionSearchResults,
	bool bWasSuccessful)
{
	if(MultiplayerPlugin == nullptr) return;

	for (FOnlineSessionSearchResult result : OnlineSessionSearchResults)
	{
		auto sessionID = result.GetSessionIdStr();
		auto user = result.Session.OwningUserName;

		FString MatchType;
		result.Session.SessionSettings.Get(FName("MatchType"), MatchType);

		if(MatchType == "Capture_The_Flag")
		{
			MultiplayerPlugin->JoinSession(result);
			return;
		}
	}
}

void AMultiplayerProjectGameMode::OnJoinSession(EOnJoinSessionCompleteResult::Type Result)
{
	if(!SessionInterface.IsValid())
	{
		mMultiplayerOnJoinSessionComplete.Broadcast(EOnJoinSessionCompleteResult::UnknownError);
		return;
	}
	mJoinSessionCompleteDelegateHandle = mSessionInterface->AddOnJoinSessionCompleteDelegate_Handle(mOnJoinSessionCompleteDelegate);
	mMultiplayerOnJoinSessionComplete.Broadcast(EOnJoinSessionCompleteResult::Success);

	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if(!mSessionInterface->JoinSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, SessionResult))
	{
		mSessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(mJoinSessionCompleteDelegateHandle);
		mMultiplayerOnJoinSessionComplete.Broadcast(EOnJoinSessionCompleteResult::UnknownError);
	}

}

void AMultiplayerProjectGameMode::OnStartSession(bool bBWasSuccessful)
{
}

void AMultiplayerProjectGameMode::OnDestroySession(bool bBWasSuccessful)
{
}
