// Fill out your copyright notice in the Description page of Project Settings.


#include "ServerListEntry.h"

#include "MultiplayerProject/SubsystemClasses/LAN_OnlineSubsystem.h"

void UServerListEntry::NativeConstruct()
{
	Super::NativeConstruct();
	btnJoin->OnClicked.AddDynamic(this, &ThisClass::OnJoin);
}

void UServerListEntry::OnJoin_Implementation()
{
	GetGameInstance()->GetSubsystem<ULAN_OnlineSubsystem>()->JoinSession(mSessionDetails);
}

void UServerListEntry::UpdateEntry(FSessionDetails SessionDetails)
{
	txtServerName->SetText(FText::FromString(SessionDetails.SessionName));
	txtPlayerCount->SetText(FText::Format(FText::FromString(TEXT("{0} / {0}")), FText::AsNumber(SessionDetails.CurrentNumberOfPlayers), FText::AsNumber(SessionDetails.MaxPlayers)));
	txtLatency->SetText(FText::Format(FText::FromString(TEXT("{0} ms")), FText::AsNumber(SessionDetails.Latency)));
}
