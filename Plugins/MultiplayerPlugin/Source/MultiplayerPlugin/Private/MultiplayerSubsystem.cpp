// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerSubsystem.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"



UMultiplayerSubsystem::UMultiplayerSubsystem()
{
	
	Completed_CreateSession = FOnCreateSessionCompleteDelegate::CreateUObject(this, &UMultiplayerSubsystem::OnComplete_CreateSession);
	Completed_FindSessions =  FOnFindSessionsCompleteDelegate::CreateUObject(this, &UMultiplayerSubsystem::OnComplete_FindSessions);
	Completed_JoinSession = FOnJoinSessionCompleteDelegate::CreateUObject(this, &UMultiplayerSubsystem::OnComplete_JoinSession);
	Completed_DestroySession = FOnDestroySessionCompleteDelegate::CreateUObject(this, &UMultiplayerSubsystem::OnComplete_DestroySession);
	Completed_StartSession = FOnStartSessionCompleteDelegate::CreateUObject(this, &UMultiplayerSubsystem::OnComplete_StartSession);

	if(const IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get())
	{
		SessionInterface = Subsystem->GetSessionInterface();
	}
}

#pragma region Ongoing Session Methods

void UMultiplayerSubsystem::CreateSession(int32 NumPublicConnections, TEnumAsByte<ETypeOfMatch> MatchType)
{
	UE_LOG(LogTemp, Warning, TEXT("Creating Session..."));

	if(!SessionInterface.IsValid()) return;

	const auto ExistingSession = SessionInterface->GetNamedSession(NAME_GameSession);

	if(ExistingSession != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Destroying Last Session..."));
		SessionInterface->DestroySession(NAME_GameSession);
	}

	Handle_CreateSessionCompleteDelegate = SessionInterface->AddOnCreateSessionCompleteDelegate_Handle(Completed_CreateSession);

	NewSessionSettings = MakeShareable(new FOnlineSessionSettings());
	NewSessionSettings->bIsLANMatch = false;	
	NewSessionSettings->NumPublicConnections = NumPublicConnections;
	NewSessionSettings->bAllowJoinInProgress = true;
	NewSessionSettings->bUseLobbiesIfAvailable = true;
	NewSessionSettings->bAllowJoinViaPresence = true;
	NewSessionSettings->bShouldAdvertise = true;
	NewSessionSettings->bUsesPresence = true;
	NewSessionSettings->Set(FName("MatchType"), GetMatchType(MatchType), EOnlineDataAdvertisementType::ViaOnlineService);
	NewSessionSettings->BuildUniqueId = 1;

	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if (!SessionInterface->CreateSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, *NewSessionSettings))
	{
		SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(Handle_CreateSessionCompleteDelegate);

		// Let the menu know that the task has failed
		OnCreateSession.Broadcast(false);
	}
}

void UMultiplayerSubsystem::FindSessions(int32 MaxSearchResults)
{
	UE_LOG(LogTemp, Warning, TEXT("Finding Session..."));
	if(SessionInterface)
	{
		Handle_FindSessionCompleteDelegate = SessionInterface->AddOnFindSessionsCompleteDelegate_Handle(Completed_FindSessions);

		NewSessionSearch = MakeShareable(new FOnlineSessionSearch());;
		NewSessionSearch->MaxSearchResults = MaxSearchResults;
		NewSessionSearch->bIsLanQuery = (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL")? true: false;
		NewSessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

		const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
		if (!SessionInterface->FindSessions(*LocalPlayer->GetPreferredUniqueNetId(), NewSessionSearch.ToSharedRef()))
		{
			SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(Handle_FindSessionCompleteDelegate);

			OnFindSessions.Broadcast(TArray<FOnlineSessionSearchResult>(), false);
		}
	}
}

void UMultiplayerSubsystem::JoinSession(const FOnlineSessionSearchResult& SessionResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Join Session..."));
	if(!SessionInterface.IsValid())
	{
		OnJoinSession.Broadcast(EOnJoinSessionCompleteResult::UnknownError);
		return;
	}
	Handle_JoinSessionCompleteDelegate = SessionInterface->AddOnJoinSessionCompleteDelegate_Handle(Completed_JoinSession);
	OnJoinSession.Broadcast(EOnJoinSessionCompleteResult::Success);

	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if(!SessionInterface->JoinSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, SessionResult))
	{
		SessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(Handle_JoinSessionCompleteDelegate);
		OnJoinSession.Broadcast(EOnJoinSessionCompleteResult::UnknownError);
	}
}

void UMultiplayerSubsystem::StartSession()
{
	UE_LOG(LogTemp, Warning, TEXT("Starting Session..."));

}

void UMultiplayerSubsystem::DestroySession()
{
	UE_LOG(LogTemp, Warning, TEXT("Destroying Session..."));

}

#pragma endregion

#pragma region On Complete Methods
void UMultiplayerSubsystem::OnComplete_CreateSession(FName SessionName, bool bSuccess)
{
	if (SessionInterface)
	{
		SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(Handle_CreateSessionCompleteDelegate);
	}
	OnCreateSession.Broadcast(bSuccess);

	UE_LOG(LogTemp, Warning, TEXT("Session Created Successfully!!"));
}

void UMultiplayerSubsystem::OnComplete_FindSessions(bool bSuccess)
{
	if(SessionInterface)
	{
		SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(Handle_FindSessionCompleteDelegate);
	}

	if(NewSessionSearch->SearchResults.Num() <= 0)
	{
		OnFindSessions.Broadcast(TArray<FOnlineSessionSearchResult>(), false);
		return;
	}
	OnFindSessions.Broadcast(NewSessionSearch->SearchResults, bSuccess);
	
	UE_LOG(LogTemp, Warning, TEXT("Session Found Successfully!!"));
}

void UMultiplayerSubsystem::OnComplete_JoinSession(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if(SessionInterface)
	{
		SessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(Handle_JoinSessionCompleteDelegate);
	}

	OnJoinSession.Broadcast(EOnJoinSessionCompleteResult::Success);
	
	UE_LOG(LogTemp, Warning, TEXT("Session Joined Successfully!!"));
}

void UMultiplayerSubsystem::OnComplete_StartSession(FName SessionName, bool bSuccess)
{

	
	UE_LOG(LogTemp, Warning, TEXT("Session Started Successfully!!"));
}

void UMultiplayerSubsystem::OnComplete_DestroySession(FName SessionName, bool bSuccess)
{
	UE_LOG(LogTemp, Warning, TEXT("Session Destroyed Successfully!!"));
}

#pragma endregion


FString UMultiplayerSubsystem::GetMatchType(ETypeOfMatch TypeOfMatch)
{
	switch (TypeOfMatch)
	{
		case Capture_The_Flag:
			return "Capture The Flag";
			break;
	}
	return {};
}
