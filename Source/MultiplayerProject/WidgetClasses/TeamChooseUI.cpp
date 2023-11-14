// Fill out your copyright notice in the Description page of Project Settings.


#include "TeamChooseUI.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MultiplayerProject/MP_HUD.h"
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

void UTeamChooseUI::InitialiseWidget(AMP_HUD* Hud, AInputController* InputController)
{
	Super::InitialiseWidget(Hud, InputController);
}

void UTeamChooseUI::OnDecisionMade()
{
	UWidgetBlueprintLibrary::SetInputMode_GameOnly(ControllerRef);
	ControllerRef->SetShowMouseCursor(false);

	DestroyWidget();
}

void UTeamChooseUI::OnTerroristButtonCLick_Implementation()
{
	if(!mHUD) return;
	mHUD->OnTeamChosen.Broadcast(ETeam::TERRORIST);
	OnDecisionMade();
}

void UTeamChooseUI::OnCounterTerroristButtonCLick_Implementation()
{
	if(!mHUD) return;
	
	mHUD->OnTeamChosen.Broadcast(ETeam::COUNTER_TERRORIST);
	OnDecisionMade();
}
