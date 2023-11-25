// Fill out your copyright notice in the Description page of Project Settings.


#include "ServerListEntry.h"

#include "GameFramework/GameStateBase.h"
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

			
	if(const AGameStateBase* GameState = GetWorld()->GetGameState())
	{
		const int CurrentPlayers = SessionDetails.MaxPlayers - GameState->PlayerArray.Num();
		txtPlayerCount->SetText(FText::Format(FText::FromString(TEXT("{0} / {0}")), FText::AsNumber(CurrentPlayers), FText::AsNumber(SessionDetails.MaxPlayers)));
	}

	txtLatency->SetText(FText::Format(FText::FromString(TEXT("{0} ms")), FText::AsNumber(SessionDetails.Latency)));
}
