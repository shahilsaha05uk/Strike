// Fill out your copyright notice in the Description page of Project Settings.


#include "LAN_OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystemUtils.h"

ULAN_OnlineSubsystem::ULAN_OnlineSubsystem()
{
	mOnCreateSessionCompleteDelegate = FOnCreateSessionCompleteDelegate::CreateUObject(this, &ULAN_OnlineSubsystem::OnCreateSessionComplete);

	mOnFindSessionsCompleteDelegate =  FOnFindSessionsCompleteDelegate::CreateUObject(this, &ULAN_OnlineSubsystem::OnFindSessionsComplete);

	mOnJoinSessionCompleteDelegate = FOnJoinSessionCompleteDelegate::CreateUObject(this, &ULAN_OnlineSubsystem::OnJoinSessionComplete);
	
	mOnStartSessionCompleteDelegate = FOnStartSessionCompleteDelegate::CreateUObject(this, &ULAN_OnlineSubsystem::OnStartSessionComplete);

	mOnDestroySessionCompleteDelegate = FOnDestroySessionCompleteDelegate::CreateUObject(this, &ULAN_OnlineSubsystem::OnDestroySessionComplete);


	const UWorld* world = UObject::GetWorld(); 
	if(Online::GetSubsystem(world))
	{
		mSessionInterface = Online::GetSessionInterface(world);
	}
}

bool ULAN_OnlineSubsystem::TryTravelToCurrentSession()
{
	mSessionInterface = Online::GetSessionInterface(GetWorld());

	if (!mSessionInterface.IsValid())
	{
		return false;
	}

	FString connectString;

	if (!mSessionInterface->GetResolvedConnectString(NAME_GameSession, connectString))
	{
		return false;
	}

	APlayerController* playerController = GetWorld()->GetFirstPlayerController();

	playerController->ClientTravel(connectString, TRAVEL_Absolute);

	return true;
}

#pragma region Create Session

void ULAN_OnlineSubsystem::CreateSession(int32 NumPublicConnections, FString MatchType)
{
	if(GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			15.0f,
			FColor::Emerald,
			FString::Printf(TEXT("Creating Session")));
	}

	if(!mSessionInterface.IsValid()) return;

	const auto ExistingSession = mSessionInterface->GetNamedSession(NAME_GameSession);

	if(ExistingSession != nullptr)
	{
		mSessionInterface->DestroySession(NAME_GameSession);
	}

	mCreateSessionCompleteDelegateHandle = mSessionInterface->AddOnCreateSessionCompleteDelegate_Handle(mOnCreateSessionCompleteDelegate);

	mLastSessionSettings = MakeShareable(new FOnlineSessionSettings());
	mLastSessionSettings->bIsLANMatch = false;
	mLastSessionSettings->NumPublicConnections = NumPublicConnections;
	mLastSessionSettings->bAllowJoinInProgress = true;
	mLastSessionSettings->bUseLobbiesIfAvailable = true;
	mLastSessionSettings->bAllowJoinViaPresence = true;
	mLastSessionSettings->bShouldAdvertise = true;
	mLastSessionSettings->bUsesPresence = true;
	mLastSessionSettings->Set(FName("MatchType"), MatchType, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	mLastSessionSettings->BuildUniqueId = 1;
	
	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if (!mSessionInterface->CreateSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, *mLastSessionSettings))
	{
		mSessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(mCreateSessionCompleteDelegateHandle);

		// Let the menu know that the task has failed
		mMultiplayerOnCreateSessionComplete.Broadcast(false);
	}
}

void ULAN_OnlineSubsystem::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (mSessionInterface)
	{
		mSessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(mCreateSessionCompleteDelegateHandle);
	}

	mMultiplayerOnCreateSessionComplete.Broadcast(bWasSuccessful);

	StartSession();
}

#pragma endregion

#pragma region Find Session

void ULAN_OnlineSubsystem::FindSessions(int32 MaxSearchResults)
{
	if(mSessionInterface)
	{
		mFindSessionsCompleteDelegateHandle = mSessionInterface->AddOnFindSessionsCompleteDelegate_Handle(mOnFindSessionsCompleteDelegate);

		mLastSessionSearch = MakeShareable(new FOnlineSessionSearch());;
		mLastSessionSearch->MaxSearchResults = MaxSearchResults;
		mLastSessionSearch->bIsLanQuery = false;
		mLastSessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

		const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
		if (!mSessionInterface->FindSessions(*LocalPlayer->GetPreferredUniqueNetId(), mLastSessionSearch.ToSharedRef()))
		{
			mSessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(mFindSessionsCompleteDelegateHandle);

			mMultiplayerOnFindSessionsComplete.Broadcast(TArray<FSessionDetails>(), false);
		}
	}

}

void ULAN_OnlineSubsystem::OnFindSessionsComplete(bool bWasSuccessful)
{
	if(mSessionInterface)
	{
		mSessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(mFindSessionsCompleteDelegateHandle);
	}

	if(mLastSessionSearch->SearchResults.Num() <= 0)
	{
		mMultiplayerOnFindSessionsComplete.Broadcast(TArray<FSessionDetails>(), false);
		return;
	}
	
	TArray<FSessionDetails> SessionDetails;
	for (FOnlineSessionSearchResult s : mLastSessionSearch->SearchResults)
	{
		FSessionDetails detail;
		detail.Result = s;
		detail.SessionID = s.GetSessionIdStr();
		detail.SessionName = s.Session.OwningUserName;
		detail.Latency = s.PingInMs;
		detail.MaxPlayers = s.Session.NumOpenPublicConnections + s.Session.NumOpenPrivateConnections;
		detail.CurrentNumberOfPlayers = s.Session.NumOpenPublicConnections - s.Session.SessionSettings.NumPublicConnections;
		SessionDetails.Add(detail);
	}
	
	mMultiplayerOnFindSessionsComplete.Broadcast(SessionDetails, bWasSuccessful);
}
#pragma endregion

#pragma region Join Session

void ULAN_OnlineSubsystem::JoinSession(FSessionDetails SessionResult)
{
	if(!mSessionInterface.IsValid())
	{
		mMultiplayerOnJoinSessionComplete.Broadcast(EOnJoinSessionCompleteResult::UnknownError);
		return;
	}

	mJoinSessionCompleteDelegateHandle = mSessionInterface->AddOnJoinSessionCompleteDelegate_Handle(mOnJoinSessionCompleteDelegate);

	mMultiplayerOnJoinSessionComplete.Broadcast(EOnJoinSessionCompleteResult::Success);

	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();

	if(!mSessionInterface->JoinSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, SessionResult.Result))
	{
		mSessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(mJoinSessionCompleteDelegateHandle);

		mMultiplayerOnJoinSessionComplete.Broadcast(EOnJoinSessionCompleteResult::UnknownError);
	}
}

void ULAN_OnlineSubsystem::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if(mSessionInterface)
	{
		mSessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(mJoinSessionCompleteDelegateHandle);
	}

	mMultiplayerOnJoinSessionComplete.Broadcast(EOnJoinSessionCompleteResult::Success);

	TryTravelToCurrentSession();
}

#pragma endregion

#pragma region Start Session

void ULAN_OnlineSubsystem::StartSession()
{
	if(!mSessionInterface.IsValid())
	{
		mMultiplayerOnStartSessionComplete.Broadcast(false);
		
		return;
	}
	
	mStartSessionCompleteDelegateHandle = mSessionInterface->AddOnStartSessionCompleteDelegate_Handle(mOnStartSessionCompleteDelegate);

	mMultiplayerOnStartSessionComplete.Broadcast(true);

	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();

	if(!mSessionInterface->StartSession(NAME_GameSession))
	{
		mSessionInterface->ClearOnStartSessionCompleteDelegate_Handle(mStartSessionCompleteDelegateHandle);

		mMultiplayerOnStartSessionComplete.Broadcast(false);
	}
}

void ULAN_OnlineSubsystem::OnStartSessionComplete(FName SessionName, bool bWasSuccessful)
{
	if(mSessionInterface)
	{
		mSessionInterface->ClearOnStartSessionCompleteDelegate_Handle(mStartSessionCompleteDelegateHandle);
	}

	mMultiplayerOnStartSessionComplete.Broadcast(bWasSuccessful);
}

#pragma endregion

#pragma region Destroy Session


void ULAN_OnlineSubsystem::DestroySession()
{
	if(!mSessionInterface.IsValid())
	{
		mMultiplayerOnDestroySessionComplete.Broadcast(false);
		return;
	}

	mDestroySessionCompleteDelegateHandle = mSessionInterface->AddOnDestroySessionCompleteDelegate_Handle(mOnDestroySessionCompleteDelegate);

	mMultiplayerOnDestroySessionComplete.Broadcast(true);

	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();

	if(!mSessionInterface->DestroySession(NAME_GameSession, mOnDestroySessionCompleteDelegate))
	{
		mSessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(mDestroySessionCompleteDelegateHandle);

		mMultiplayerOnDestroySessionComplete.Broadcast(false);
	}
}

void ULAN_OnlineSubsystem::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	if(mSessionInterface)
	{
		mSessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(mDestroySessionCompleteDelegateHandle);
	}
	mMultiplayerOnDestroySessionComplete.Broadcast(bWasSuccessful);
}

#pragma endregion
