// Fill out your copyright notice in the Description page of Project Settings.


#include "Session_GameInstanceComponent.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystemUtils.h"

USession_GameInstanceComponent::USession_GameInstanceComponent()
{
	CompleteDelegate_CreateSession = FOnCreateSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnCreateSessionCompleted);
	CompleteDelegate_UpdateSession = FOnUpdateSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnUpdateSessionCompleted);
	CompleteDelegate_StartSession = FOnStartSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnStartSessionCompleted);
	CompleteDelegate_EndSession = FOnEndSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnUpdateSessionCompleted);
	CompleteDelegate_DestroySession = FOnDestroySessionCompleteDelegate::CreateUObject(this, &ThisClass::OnDestroySessionCompleted);
	
	CompleteDelegate_FindSession =  FOnFindSessionsCompleteDelegate::CreateUObject(this, &ThisClass::OnFindSessionsCompleted);
	CompleteDelegate_JoinSession = FOnJoinSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnJoinSessionCompleted);
}

void USession_GameInstanceComponent::Initialize(FSubsystemCollectionBase& Collection)
{
	mSessionInterface = Online::GetSessionInterface(GetWorld());
	
	if (!mSessionInterface.IsValid())
	{
		Event_OnCreateSessionComplete.Broadcast(false);
		return;
	}

	Super::Initialize(Collection);
}

#pragma region Unused Methods
void USession_GameInstanceComponent::UpdateSession()
{
}

void USession_GameInstanceComponent::EndSession()
{
}

void USession_GameInstanceComponent::DestroySession()
{
}


void USession_GameInstanceComponent::OnUpdateSessionCompleted(FName SessionName, bool Successful)
{
}

void USession_GameInstanceComponent::OnEndSessionCompleted(FName SessionName, bool Successful)
{
}

void USession_GameInstanceComponent::OnDestroySessionCompleted(FName SessionName, bool Successful)
{
}

bool USession_GameInstanceComponent::TryTravelToCurrentSession()
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


#pragma endregion



#pragma region Create Session
void USession_GameInstanceComponent::CreateSession(FMatchDetails MatchDetails)
{
	
	mSessionInterface = Online::GetSessionInterface(GetWorld());
	if (!mSessionInterface.IsValid())
	{
		Event_OnCreateSessionComplete.Broadcast(false);
		return;
	}

	mMatchDetails = MatchDetails;

	LastSessionSettings = MakeShareable(new FOnlineSessionSettings());
	LastSessionSettings->NumPublicConnections = mMatchDetails.MaxPlayers;
	LastSessionSettings->bIsLANMatch = mMatchDetails.bIsLanMatch;
	LastSessionSettings->bUsesPresence = true;
	LastSessionSettings->bShouldAdvertise = true;
	LastSessionSettings->bIsDedicated = false;
	LastSessionSettings->bAllowJoinInProgress = true;
	LastSessionSettings->bAllowJoinViaPresence = true;
	LastSessionSettings->bAllowJoinViaPresenceFriendsOnly = true;
	LastSessionSettings->bAllowInvites = true;

	LastSessionSettings->Set(SETTING_MAPNAME, FString("Level1"), EOnlineDataAdvertisementType::ViaOnlineService);
	
	Handle_CreateSessionCompleteDelegate = mSessionInterface->AddOnCreateSessionCompleteDelegate_Handle(CompleteDelegate_CreateSession);

	const ULocalPlayer* localPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	
	if (!mSessionInterface->CreateSession(*localPlayer->GetPreferredUniqueNetId(), NAME_GameSession, *LastSessionSettings))
	{
		mSessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(Handle_CreateSessionCompleteDelegate);

		Event_OnCreateSessionComplete.Broadcast(false);
	}

	StartSession();
}

void USession_GameInstanceComponent::OnCreateSessionCompleted(FName SessionName, bool Successful)
{
	UE_LOG(LogTemp, Warning, TEXT("Session Created!!"));

	if(!Successful)
	{
		mSessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(Handle_CreateSessionCompleteDelegate);
	}

	Event_OnCreateSessionComplete.Broadcast(Successful);
	
}

#pragma endregion

#pragma region Start Session

void USession_GameInstanceComponent::StartSession()
{
	mSessionInterface = Online::GetSessionInterface(GetWorld());
	if (!mSessionInterface.IsValid())
	{
		Event_OnStartSessionComplete.Broadcast(false);
		return;
	}

	Handle_StartSessionCompleteDelegate = mSessionInterface->AddOnStartSessionCompleteDelegate_Handle(CompleteDelegate_StartSession);

	if (!mSessionInterface->StartSession(NAME_GameSession))
	{
		mSessionInterface->ClearOnStartSessionCompleteDelegate_Handle(Handle_StartSessionCompleteDelegate);

		Event_OnStartSessionComplete.Broadcast(false);
	}
}

void USession_GameInstanceComponent::OnStartSessionCompleted(FName SessionName, bool Successful)
{
	mSessionInterface = Online::GetSessionInterface(GetWorld());
	if (mSessionInterface)
	{
		mSessionInterface->ClearOnStartSessionCompleteDelegate_Handle(Handle_StartSessionCompleteDelegate);
	}

	Event_OnStartSessionComplete.Broadcast(Successful);
}


#pragma endregion

#pragma region Find Session

void USession_GameInstanceComponent::FindSessions(int MaxSearchResults, bool IsLanQuery)
{
	mSessionInterface = Online::GetSessionInterface(GetWorld());
	
	if(!mSessionInterface.IsValid())
	{
		Event_OnFindSessionsComplete.Broadcast(TArray<FSessionDetails>(), false);
		return;
	}

	Handle_FindSessionsCompleteDelegate = mSessionInterface->AddOnFindSessionsCompleteDelegate_Handle(CompleteDelegate_FindSession);

	LastSessionSearch = MakeShareable(new FOnlineSessionSearch());
	LastSessionSearch->MaxSearchResults = MaxSearchResults;
	LastSessionSearch->bIsLanQuery = IsLanQuery;
	LastSessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

	
	const ULocalPlayer* localPlayer = GetWorld()->GetFirstLocalPlayerFromController();

	if(!mSessionInterface->FindSessions(*localPlayer->GetPreferredUniqueNetId(), LastSessionSearch.ToSharedRef()))
	{
		mSessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(Handle_FindSessionsCompleteDelegate);

		Event_OnFindSessionsComplete.Broadcast(TArray<FSessionDetails>(), false);
	}
	
}


void USession_GameInstanceComponent::OnFindSessionsCompleted(bool Successful)
{

	const auto SearchResults = LastSessionSearch->SearchResults;

	if(SearchResults.Num() <= 0)
	{
		Event_OnFindSessionsComplete.Broadcast(TArray<FSessionDetails>(), Successful);
	}

	TArray<FSessionDetails> SessionDetails;

	if(SessionDetails.Num() >0)
	{
		SessionDetails.Empty();
	}

	
	for (FOnlineSessionSearchResult s : SearchResults)
	{
		FSessionDetails detail;

		detail.Result = s;
		detail.SessionID = s.GetSessionIdStr();
		detail.Latency = s.PingInMs;
		detail.CurrentNumberOfPlayers = s.Session.NumOpenPublicConnections;

		SessionDetails.Add(detail);
	}


	
	Event_OnFindSessionsComplete.Broadcast(SessionDetails, Successful);
}

#pragma endregion

#pragma region Join Session

void USession_GameInstanceComponent::JoinSession(const FOnlineSessionSearchResult& SessionResult)
{
	const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
	if (!sessionInterface.IsValid())
	{
		Event_OnJoinGameSessionComplete.Broadcast(EOnJoinSessionCompleteResult::Type::UnknownError);
		return;
	}

	Handle_JoinSessionCompleteDelegate = sessionInterface->AddOnJoinSessionCompleteDelegate_Handle(CompleteDelegate_JoinSession);

	const ULocalPlayer* localPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if (!sessionInterface->JoinSession(*localPlayer->GetPreferredUniqueNetId(), NAME_GameSession, SessionResult))
	{
		sessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(Handle_JoinSessionCompleteDelegate);

		Event_OnJoinGameSessionComplete.Broadcast(EOnJoinSessionCompleteResult::Type::UnknownError);
	}
}

void USession_GameInstanceComponent::OnJoinSessionCompleted(FName SessionName,
	EOnJoinSessionCompleteResult::Type Result)
{
	mSessionInterface = Online::GetSessionInterface(GetWorld());
	if (mSessionInterface)
	{
		mSessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(Handle_JoinSessionCompleteDelegate);
	}
	
	Event_OnJoinGameSessionComplete.Broadcast(EOnJoinSessionCompleteResult::Success);

	TryTravelToCurrentSession();

	UE_LOG(LogTemp, Warning, TEXT("Joined Session Successfully"));
}

#pragma endregion
