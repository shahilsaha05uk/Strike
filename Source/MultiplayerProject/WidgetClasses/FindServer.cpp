// Fill out your copyright notice in the Description page of Project Settings.


#include "FindServer.h"

#include "ServerListEntry.h"
#include "Components/Button.h"
#include "Components/ListView.h"

void UFindServer::NativeConstruct()
{
	if(UGameInstance* GameInstance = GetGameInstance())
	{
		mSessionComp = GameInstance->GetSubsystem<USession_GameInstanceComponent>();
	}

	if(mSessionComp)
	{
		mSessionComp->Event_OnFindSessionsComplete.AddUObject(this, &ThisClass::OnFindSessionComplete);
	}
	
	Super::NativeConstruct();

	btnConnect->OnClicked.AddDynamic(this, &UFindServer::OnConnect);

}

void UFindServer::OnFindSessionComplete_Implementation(const TArray<FSessionDetails>& OnlineSessionSearchResults,
	bool bSuccessful)
{

}

void UFindServer::OnConnect_Implementation()
{

}
