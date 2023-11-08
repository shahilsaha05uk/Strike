// Fill out your copyright notice in the Description page of Project Settings.


#include "FindServer.h"
#include "Components/Button.h"
#include "Components/ListView.h"
#include "MultiplayerProject/Multiplayer_GameInstance.h"
#include "MultiplayerProject/SubsystemClasses/LAN_OnlineSubsystem.h"

void UFindServer::NativeConstruct()
{
	Super::NativeConstruct();
	
	MultiplayerSessionsSubsystem = GetGameInstance()->GetSubsystem<ULAN_OnlineSubsystem>();
	Cast<UMultiplayer_GameInstance>(GetGameInstance())->OnFindSessionComplete.AddDynamic(this, &ThisClass::OnFindSessionComplete);
	btnRefresh->OnClicked.AddDynamic(this, &UFindServer::OnRefresh);
	CloseButton->OnClicked.AddDynamic(this, &ThisClass::DestroyWidget);

	//Refresh on Construct
	OnRefresh();
}


void UFindServer::OnFindSessionComplete(TArray<FSessionDetails> SessionDetails)
{
	mSessionDetails = SessionDetails;
	UpdateList();
}

void UFindServer::OnRefresh_Implementation()
{
	if(mServerList->GetNumItems() > 0)
	{
		mServerList->ClearListItems();
	}
	
	MultiplayerSessionsSubsystem->FindSessions(10);
}

void UFindServer::UpdateList_Implementation()
{
	
}
