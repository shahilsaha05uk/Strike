// Fill out your copyright notice in the Description page of Project Settings.


#include "TeamChooseUI.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MultiplayerProject/MP_PlayerState.h"
#include "MultiplayerProject/Multiplayer_GameInstance.h"
#include "MultiplayerProject/InterfaceClasses/ControllerInterface.h"

void UTeamChooseUI::NativeConstruct()
{
	Super::NativeConstruct();

	GameInstance = Cast<UMultiplayer_GameInstance>(GetGameInstance());
	PlayerStateRef = Cast<AMP_PlayerState>(ControllerRef->PlayerState);
	
	btnCounterTerrorist->OnClicked.AddDynamic(this, &ThisClass::OnCounterTerroristButtonCLick);
	btnTerrorist->OnClicked.AddDynamic(this, &ThisClass::OnTerroristButtonCLick);
}

void UTeamChooseUI::UpdatePlayerState_Implementation(ETeam Team)
{
	UWidgetBlueprintLibrary::SetInputMode_GameOnly(ControllerRef);
	ControllerRef->SetShowMouseCursor(false);

	FPlayerDetails PlayerDetails;
	PlayerDetails.Team = Team;
	PlayerDetails.CurrentMoney = GameInstance->mMatchDetails.StartingMoney;

	if (PlayerStateRef) PlayerStateRef->mPlayerDetails = PlayerDetails;

	if(UKismetSystemLibrary::DoesImplementInterface(ControllerRef, UControllerInterface::StaticClass()))
	{
		IControllerInterface::Execute_SpawnPawn(ControllerRef);
	}
	
	DestroyWidget();

}

void UTeamChooseUI::OnTerroristButtonCLick_Implementation()
{
	UpdatePlayerState(ETeam::TERRORIST);
}

void UTeamChooseUI::OnCounterTerroristButtonCLick_Implementation()
{
	UpdatePlayerState(ETeam::COUNTER_TERRORIST);
}
