// Fill out your copyright notice in the Description page of Project Settings.


#include "HostServer.h"

#include "Components/Button.h"
#include "CustomWidgets/EntryWidget1.h"
#include "MultiplayerProject/Multiplayer_GameInstance.h"
#include "MultiplayerProject/SubsystemClasses/LAN_OnlineSubsystem.h"

void UHostServer::NativeConstruct()
{
	Super::NativeConstruct();
	
	mMultiplayerSubsystem = GetGameInstance()->GetSubsystem<ULAN_OnlineSubsystem>();
	
	if(btnHost) btnHost->OnClicked.AddDynamic(this, &UHostServer::OnHost);
	
	if(CloseButton) CloseButton->OnClicked.AddDynamic(this, &ThisClass::DestroyWidget);
}

void UHostServer::OnHost_Implementation()
{
	mMatchDetails.MaxPlayers = MaxPlayerEntry->FieldValue;
	
	mMatchDetails.StartingMoney = StartingMoneyEntry->FieldValue;

	Cast<UMultiplayer_GameInstance>(GetGameInstance())->SetSessionDetails(mMatchDetails);
	
	mMultiplayerSubsystem->CreateSession(mMatchDetails.MaxPlayers, "CaptureTheFlag");
}
