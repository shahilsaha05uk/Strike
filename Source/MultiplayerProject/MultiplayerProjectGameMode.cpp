// Copyright Epic Games, Inc. All Rights Reserved.

#include "MultiplayerProjectGameMode.h"
#include "MultiplayerSubsystem.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include "Kismet/BlueprintPathsLibrary.h"
#include "Kismet/KismetSystemLibrary.h"


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
		MultiplayerPlugin->OnCreateSession.AddDynamic(this, &ThisClass::OnCreateSession);
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
			//World->ServerTravel(FString("/Game/Maps/Level1?listen"));
			//World->ServerTravel(mLevelMapRef->GetPathName().);

			const FString path = UBlueprintPathsLibrary::GetBaseFilename(UKismetSystemLibrary::Conv_SoftObjectReferenceToString(mLevelMapRef), false);
			UE_LOG(LogTemp, Warning, TEXT("Map Path: %s"), *path);
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
	if(const IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get())
	{
		const IOnlineSessionPtr SessionInterface = Subsystem->GetSessionInterface();

		if(SessionInterface.IsValid())
		{
			FString Address;

			SessionInterface->GetResolvedConnectString(NAME_GameSession, Address);
			APlayerController* PC = GetGameInstance()->GetFirstLocalPlayerController();

			if(PC)
			{
				PC->ClientTravel(Address, TRAVEL_Absolute);
			}
		}
	}
}

void AMultiplayerProjectGameMode::OnStartSession(bool bBWasSuccessful)
{
}

void AMultiplayerProjectGameMode::OnDestroySession(bool bBWasSuccessful)
{
}
