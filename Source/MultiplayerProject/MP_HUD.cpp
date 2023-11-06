// Fill out your copyright notice in the Description page of Project Settings.


#include "MP_HUD.h"

#include "WidgetClasses/BuyMenu.h"
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
	UBaseWidget* Widget = (GetWidget(WidgetToSpawn) == nullptr)
		                      ? WidgetCreator(WidgetToSpawn)
		                      : GetWidget(WidgetToSpawn);

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
	UBaseWidget* Widget = GetWidget(WidgetToDestroy);

	if(Widget != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Widget is valid"));
		
		if(WidgetMap.Contains(WidgetToDestroy))
		{
			WidgetMap[WidgetToDestroy]->DestroyWidget();
			WidgetMap.Remove(WidgetToDestroy);
		}
		
	}
}

UBaseWidget* AMP_HUD::GetWidget_Implementation(EWidgetType WidgetToGet)
{
	UBaseWidget* Widget = nullptr;
	if(WidgetReferenceCheck(Widget, WidgetToGet))
	{
		UE_LOG(LogTemp, Warning, TEXT("Widget is valid"));

	}
	return Widget;
}

bool AMP_HUD::WidgetReferenceCheck(UBaseWidget* &WidgetRef, EWidgetType WidgetToSpawn)
{
	WidgetRef = nullptr;

	if (!WidgetMap.Contains(WidgetToSpawn)) return false;

	WidgetRef = WidgetMap[WidgetToSpawn];

	return true;

}
