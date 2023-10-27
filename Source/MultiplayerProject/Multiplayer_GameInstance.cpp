// Fill out your copyright notice in the Description page of Project Settings.


#include "Multiplayer_GameInstance.h"

#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include "Session_GameInstanceComponent.h"


void UMultiplayer_GameInstance::Init()
{
	Super::Init();
	SessionComp = GetSubsystem<USession_GameInstanceComponent>();

	if(SessionComp != nullptr)
	{
		SessionComp->Event_OnCreateSessionComplete.AddDynamic(this, &ThisClass::OnCreateSessionComplete);
		SessionComp->Event_OnStartSessionComplete.AddDynamic(this, &ThisClass::OnSessionStartComplete);
		SessionComp->Event_OnJoinGameSessionComplete.AddUObject(this, &ThisClass::OnJoinSessionComplete);
	}
}


void UMultiplayer_GameInstance::CreateSession_Implementation(FMatchDetails MatchDetails)
{
	SessionComp->CreateSession(MatchDetails);
}
void UMultiplayer_GameInstance::FindSession_Implementation(int MaxSearchResults, bool IsLanQuery)
{
	SessionComp->FindSessions(MaxSearchResults, IsLanQuery);
}

void UMultiplayer_GameInstance::JoinSession_Implementation(FSessionDetails SessionDetails)
{
	SessionComp->JoinSession(SessionDetails.Result);
}



void UMultiplayer_GameInstance::OnCreateSessionComplete_Implementation(bool bSuccessful)
{
	
}

void UMultiplayer_GameInstance::OnSessionStartComplete_Implementation(bool bSuccessful)
{
	
}

void UMultiplayer_GameInstance::OnJoinSessionComplete(EOnJoinSessionCompleteResult::Type Result)
{

}
