// Fill out your copyright notice in the Description page of Project Settings.


#include "OnlineMenu.h"

#include "../../../Plugins/MultiplayerPlugin/Source/MultiplayerPlugin/Public/MultiplayerSubsystem.h"

void UOnlineMenu::NativeConstruct()
{
	btnConnect->OnClicked.AddDynamic(this, &UOnlineMenu::OnConnect);
	btnHost->OnClicked.AddDynamic(this, &UOnlineMenu::OnHost);

	if(UGameInstance* GameInstance = GetGameInstance())
	{
		MultiplayerPlugin = GameInstance->GetSubsystem<UMultiplayerSubsystem>();
	}
	
	Super::NativeConstruct();
}

void UOnlineMenu::OnConnect_Implementation()
{
	MultiplayerPlugin->FindSessions(MaxConnections);
}

void UOnlineMenu::OnHost_Implementation()
{
	MultiplayerPlugin->CreateSession(MaxConnections, MatchType);
}
