// Fill out your copyright notice in the Description page of Project Settings.


#include "Multiplayer_GameInstance.h"

#include "MP_HUD.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSubsystem.h"
#include "SubsystemClasses/MultiplayerSessionsSubsystem.h"

void UMultiplayer_GameInstance::Init()
{
	Super::Init();

	mMultiplayerSessionsSubsystem = GetSubsystem<UMultiplayerSessionsSubsystem>();
	if(mMultiplayerSessionsSubsystem)
	{
		mMultiplayerSessionsSubsystem->mMultiplayerOnCreateSessionComplete.AddDynamic(this, &ThisClass::OnCreateSession);
		mMultiplayerSessionsSubsystem->mMultiplayerOnFindSessionsComplete.AddUObject(this,  &ThisClass::OnFindSessions);
		mMultiplayerSessionsSubsystem->mMultiplayerOnJoinSessionComplete.AddUObject(this,  &ThisClass::OnJoinSession);
		mMultiplayerSessionsSubsystem->mMultiplayerOnStartSessionComplete.AddDynamic(this, &ThisClass::OnStartSession);
		mMultiplayerSessionsSubsystem->mMultiplayerOnDestroySessionComplete.AddDynamic(this, &ThisClass::OnDestroySession);
	}
}

void UMultiplayer_GameInstance::OnCreateSession(bool bWasSuccessful)
{
	if(bWasSuccessful)
	{
		UWorld* World = GetWorld();

		if(World)
		{
			FString LevelPath = GetLevelPath(mTravelMap, true);
			
			UE_LOG(LogTemp, Warning, TEXT("Path: %s"), *LevelPath);
			World->ServerTravel(LevelPath);
		}
	}

	GetSubsystem<UMultiplayerSessionsSubsystem>()->StartSession();
}

void UMultiplayer_GameInstance::OnFindSessions(const TArray<FSessionDetails>& OnlineSessionSearchResults,
	bool bWasSuccessful)
{
	if(bWasSuccessful)	OnFindSessionComplete.Broadcast(OnlineSessionSearchResults);
}

void UMultiplayer_GameInstance::OnJoinSession(EOnJoinSessionCompleteResult::Type Result)
{
	if(const IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get())
	{
		const IOnlineSessionPtr SessionInterface = Subsystem->GetSessionInterface();

		if(SessionInterface.IsValid())
		{
			FString Address;

			SessionInterface->GetResolvedConnectString(NAME_GameSession, Address);
			APlayerController* PC = GetFirstLocalPlayerController();

			if(PC)
			{
				PC->ClientTravel(Address, TRAVEL_Absolute);
			}
		}
	}
}

void UMultiplayer_GameInstance::OnStartSession(bool bBWasSuccessful)
{
}

void UMultiplayer_GameInstance::OnDestroySession(bool bWasSuccessful)
{
	if(const IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get())
	{
		const IOnlineSessionPtr SessionInterface = Subsystem->GetSessionInterface();

		if(SessionInterface.IsValid())
		{
			FString LevelPath = GetLevelPath(mMainMenuLevel, true);
			
			APlayerController* PC = GetFirstLocalPlayerController();

			if(PC)
			{
				PC->ClientTravel(LevelPath, TRAVEL_Absolute);
			}
		}
	}
}

FString UMultiplayer_GameInstance::GetLevelPath(TSoftObjectPtr<UWorld> Map, bool shouldListen)
{
	return FPaths::GetBaseFilename(Map.ToString());
}
