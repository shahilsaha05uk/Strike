// Fill out your copyright notice in the Description page of Project Settings.


#include "HostServer.h"

#include "MultiplayerSubsystem.h"
#include "Components/Button.h"

void UHostServer::NativeConstruct()
{

	if(UGameInstance* GameInstance = GetGameInstance())
	{
		MultiplayerPlugin = GameInstance->GetSubsystem<UMultiplayerSubsystem>();
	}

	Super::NativeConstruct();

	if(btnHost) btnHost->OnClicked.AddDynamic(this, &UHostServer::OnHost);
}

void UHostServer::OnHost_Implementation()
{
	MultiplayerPlugin->CreateSession(MaxConnections, MatchType);

}
