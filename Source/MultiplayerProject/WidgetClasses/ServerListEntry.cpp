// Fill out your copyright notice in the Description page of Project Settings.


#include "ServerListEntry.h"

void UServerListEntry::UpdateEntry(FSessionDetails SessionDetails)
{
	txtServerName->SetText(FText::FromString(SessionDetails.SessionName));
	txtPlayerCount->SetText(FText::Format(FText::FromString(TEXT("{0} / {0}")), FText::AsNumber(SessionDetails.CurrentNumberOfPlayers), FText::AsNumber(SessionDetails.MaxPlayers)));
	txtLatency->SetText(FText::Format(FText::FromString(TEXT("{0} ms")), FText::AsNumber(SessionDetails.Latency)));
}
