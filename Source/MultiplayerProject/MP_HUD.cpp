// Fill out your copyright notice in the Description page of Project Settings.


#include "MP_HUD.h"

#include "WidgetClasses/MainMenu.h"
#include "WidgetClasses/PlayerHUD.h"


void AMP_HUD::BeginPlay()
{
	Init();
	Super::BeginPlay();
}

void AMP_HUD::Init_Implementation()
{

}

UBaseWidget* AMP_HUD::WidgetInitialiser_Implementation(EWidgetType WidgetToSpawn)
{
	/*
	UBaseWidget* WidgetRef = WidgetCreator(WidgetToSpawn);

	if(mWidgetMap.Contains(WidgetToSpawn))
	{
		return mWidgetMap[WidgetToSpawn];
	}

	if(mWidgetMap.IsEmpty() || !mWidgetMap.Contains(WidgetToSpawn))
	{
		mWidgetMap.Add(WidgetToSpawn, WidgetRef);
	}

	
	*/
	return WidgetCreator(WidgetToSpawn);

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
	}
	
	return widget;
}
