// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerSessionsSubsystem.h"

#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include "MultiplayerProject/StructClass.h"

UMultiplayerSessionsSubsystem::UMultiplayerSessionsSubsystem()
{
	mOnCreateSessionCompleteDelegate = FOnCreateSessionCompleteDelegate::CreateUObject(this, &UMultiplayerSessionsSubsystem::OnCreateSessionComplete);
	mOnFindSessionsCompleteDelegate =  FOnFindSessionsCompleteDelegate::CreateUObject(this, &UMultiplayerSessionsSubsystem::OnFindSessionsComplete);
	mOnJoinSessionCompleteDelegate = FOnJoinSessionCompleteDelegate::CreateUObject(this, &UMultiplayerSessionsSubsystem::OnJoinSessionComplete);
	
	mOnStartSessionCompleteDelegate = FOnStartSessionCompleteDelegate::CreateUObject(this, &UMultiplayerSessionsSubsystem::OnStartSessionComplete);
	mOnDestroySessionCompleteDelegate = FOnDestroySessionCompleteDelegate::CreateUObject(this, &UMultiplayerSessionsSubsystem::OnDestroySessionComplete);
	
	if (const IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get())
		mSessionInterface = Subsystem->GetSessionInterface();
	
}
#pragma region These Methods are called by the Menu

void UMultiplayerSessionsSubsystem::CreateSession(int32 NumPublicConnections, FString MatchType)
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
	
	mLastSessionSettings->bIsLANMatch = (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL")? true: false;
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

void UMultiplayerSessionsSubsystem::FindSessions(int32 MaxSearchResults)
{
	if(mSessionInterface)
	{
		mFindSessionsCompleteDelegateHandle = mSessionInterface->AddOnFindSessionsCompleteDelegate_Handle(mOnFindSessionsCompleteDelegate);

		mLastSessionSearch = MakeShareable(new FOnlineSessionSearch());;
		mLastSessionSearch->MaxSearchResults = MaxSearchResults;
		mLastSessionSearch->bIsLanQuery = (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL")? true: false;
		mLastSessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

		const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
		if (!mSessionInterface->FindSessions(*LocalPlayer->GetPreferredUniqueNetId(), mLastSessionSearch.ToSharedRef()))
		{
			mSessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(mFindSessionsCompleteDelegateHandle);

			mMultiplayerOnFindSessionsComplete.Broadcast(TArray<FSessionDetails>(), false);
		}
	}
}

void UMultiplayerSessionsSubsystem::JoinSession(FSessionDetails SessionResult)
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

void UMultiplayerSessionsSubsystem::StartSession()
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

void UMultiplayerSessionsSubsystem::DestroySession()
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

#pragma endregion

#pragma region These Methods are binded to the Delegates in the constructor

void UMultiplayerSessionsSubsystem::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (mSessionInterface)
	{
		mSessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(mCreateSessionCompleteDelegateHandle);
	}

	mMultiplayerOnCreateSessionComplete.Broadcast(bWasSuccessful);
}

void UMultiplayerSessionsSubsystem::OnFindSessionsComplete(bool bWasSuccessful)
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

void UMultiplayerSessionsSubsystem::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if(mSessionInterface)
	{
		mSessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(mJoinSessionCompleteDelegateHandle);
	}

	mMultiplayerOnJoinSessionComplete.Broadcast(EOnJoinSessionCompleteResult::Success);
}

void UMultiplayerSessionsSubsystem::OnStartSessionComplete(FName SessionName, bool bWasSuccessful)
{
	if(mSessionInterface)
	{
		mSessionInterface->ClearOnStartSessionCompleteDelegate_Handle(mStartSessionCompleteDelegateHandle);
	}

	mMultiplayerOnStartSessionComplete.Broadcast(bWasSuccessful);
}

void UMultiplayerSessionsSubsystem::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	if(mSessionInterface)
	{
		mSessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(mDestroySessionCompleteDelegateHandle);
	}
	mMultiplayerOnDestroySessionComplete.Broadcast(bWasSuccessful);
}

#pragma endregion
