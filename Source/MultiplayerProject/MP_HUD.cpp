// Fill out your copyright notice in the Description page of Project Settings.


#include "MP_HUD.h"

#include "InterfaceClasses/ControllerInterface.h"
#include "Kismet/KismetSystemLibrary.h"
#include "WidgetClasses/BuyMenu.h"
#include "WidgetClasses/FindServer.h"
#include "WidgetClasses/HostServer.h"
#include "WidgetClasses/MainMenu.h"
#include "WidgetClasses/PauseMenu.h"
#include "WidgetClasses/PlayerHUD.h"
#include "WidgetClasses/TeamChooseUI.h"


void AMP_HUD::BeginPlay()
{
	Init();
	Super::BeginPlay();
}

void AMP_HUD::Init_Implementation()
{
	//mWidgetMap = new TMap<TEnumAsByte<EWidgetType>, UBaseWidget*>();
}

UBaseWidget* AMP_HUD::WidgetInitialiser_Implementation(EWidgetType WidgetToSpawn)
{
	UBaseWidget* Widget = (Execute_GetWidget(this, WidgetToSpawn) == nullptr)
		                      ? WidgetCreator(WidgetToSpawn)
		                      : Execute_GetWidget(this, WidgetToSpawn);

	// Initialise the widget
	switch (WidgetToSpawn) {
	case TEAM_MENU:
		Widget->InitialiseWidget(this);
		OnTeamChosen.AddDynamic(this, &ThisClass::OnDecisionMade);
		
		break;
	case PLAYER_HUD:
		break;
	case MAIN_MENU:
		break;
	case PAUSE_MENU:
		break;
	case SHOP_MENU:
		break;
	case FIND_SERVER_MENU:
		break;
	case HOST_SERVER_MENU:
		break;
	default: ;
	}


	UpdateWidgetMap(Widget, WidgetToSpawn);
	
	return Widget;
}

UBaseWidget* AMP_HUD::WidgetCreator(EWidgetType WidgetToSpawn)
{
	UBaseWidget* widget = nullptr;
	
	switch (WidgetToSpawn) {
	case PLAYER_HUD:
		widget = CreateWidget<UPlayerHUD>(GetOwningPlayerController(), PlayerHudClass);
		break;
	case MAIN_MENU:
		widget = CreateWidget<UMainMenu>(GetOwningPlayerController(), MainMenuClass);
		break;
	case PAUSE_MENU:
		widget = CreateWidget<UPauseMenu>(GetOwningPlayerController(), PauseMenuClass);
		break;
	case SHOP_MENU:
		widget = CreateWidget<UBuyMenu>(GetOwningPlayerController(), ShopMenuClass);
		break;
	case TEAM_MENU:
		widget = CreateWidget<UTeamChooseUI>(GetOwningPlayerController(), TeamMenuClass);
		break;
	case HOST_SERVER_MENU:
		widget = CreateWidget<UHostServer>(GetOwningPlayerController(), HostServerMenuClass);
		break;
	case FIND_SERVER_MENU:
		widget = CreateWidget<UFindServer>(GetOwningPlayerController(), FindServerMenuClass);
		break;
	}
	
	return widget;
}

void AMP_HUD::UpdateWidgetMap(UBaseWidget* Widget, EWidgetType WidgetToUpdate)
{
	if(WidgetMap.Contains(WidgetToUpdate)) return;

	WidgetMap.Add(WidgetToUpdate, Widget);
}

void AMP_HUD::WidgetDestroyer_Implementation(EWidgetType WidgetToDestroy)
{
	switch (WidgetToDestroy) {
	case PLAYER_HUD:
		break;
	case MAIN_MENU:
		break;
	case PAUSE_MENU:
		break;
	case SHOP_MENU:
		break;
	case TEAM_MENU:
		OnTeamChosen.RemoveDynamic(this, &ThisClass::OnDecisionMade);
		break;
	case FIND_SERVER_MENU:
		break;
	case HOST_SERVER_MENU:
		break;
	default: ;
	}
	
	if(WidgetMap.Contains(WidgetToDestroy))
	{
		WidgetMap[WidgetToDestroy]->RemoveFromParent();
		WidgetMap.Remove(WidgetToDestroy);
	}
}

UBaseWidget* AMP_HUD::GetWidget_Implementation(EWidgetType WidgetToGet)
{
	UBaseWidget* Widget = nullptr;
	return Widget;
}

bool AMP_HUD::WidgetReferenceCheck(UBaseWidget* &WidgetRef, EWidgetType WidgetToSpawn)
{
	WidgetRef = nullptr;

	if (!WidgetMap.Contains(WidgetToSpawn)) return false;

	WidgetRef = WidgetMap[WidgetToSpawn];

	return true;

}

UPlayerHUD* AMP_HUD::OnPawnSpawn_Implementation()
{
	UPlayerHUD* tmpHUD = Cast<UPlayerHUD>(Execute_WidgetInitialiser(this, EWidgetType::PLAYER_HUD));

	return tmpHUD;
}

#pragma region Team Methods


void AMP_HUD::OnDecisionMade_Implementation(UDA_CharacterMeshDetails* CharacterDetails)
{
	APlayerController* PC = GetOwningPlayerController();

	if(UKismetSystemLibrary::DoesImplementInterface(PC, UControllerInterface::StaticClass()))
	{
		IControllerInterface::Execute_PawnSetup(PC, CharacterDetails, false);
	}
}

#pragma endregion
