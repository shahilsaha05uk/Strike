// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWidget.h"

#include "GameFramework/HUD.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MultiplayerProject/InterfaceClasses/HUDInterface.h"


void UBaseWidget::NativeConstruct()
{
	Super::NativeConstruct();

	mHudRef = GetOwningPlayer()->GetHUD();
}

void UBaseWidget::DestroyWidget_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Destroy Widget"));

	if(UKismetSystemLibrary::DoesImplementInterface(mHudRef, UHUDInterface::StaticClass()))
	{
		IHUDInterface::Execute_WidgetDestroyer(mHudRef, mWidgetType);
	}
}

void UBaseWidget::QuitGame_Implementation()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetOwningPlayer(), EQuitPreference::Quit, false);
}
