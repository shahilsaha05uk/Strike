// Fill out your copyright notice in the Description page of Project Settings.


#include "FindServer.h"

#include "MultiplayerSubsystem.h"
#include "ServerListEntry.h"
#include "Components/Button.h"
#include "Components/ListView.h"

void UFindServer::NativeConstruct()
{

	if(UGameInstance* GameInstance = GetGameInstance())
	{
		MultiplayerPlugin = GameInstance->GetSubsystem<UMultiplayerSubsystem>();
	}
	Super::NativeConstruct();

	btnConnect->OnClicked.AddDynamic(this, &UFindServer::OnConnect);

}

void UFindServer::OnConnect_Implementation()
{
	MultiplayerPlugin->FindSessions(MaxConnections);
}
