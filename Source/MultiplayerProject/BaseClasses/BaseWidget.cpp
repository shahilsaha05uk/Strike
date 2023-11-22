// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWidget.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "GameFramework/HUD.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MultiplayerProject/InterfaceClasses/HUDInterface.h"


void UBaseWidget::NativeConstruct()
{
	Super::NativeConstruct();

	mHudRef = GetOwningPlayer()->GetHUD();
}

void UBaseWidget::InitialiseWidget(AMP_HUD* Hud, AInputController* InputController)
{
	mHUD = Hud;
	mInputController = InputController;

}

void UBaseWidget::PauseGame_Implementation(bool Value)
{
	GetOwningPlayer()->SetPause(Value);
}

void UBaseWidget::ResumeGame_Implementation()
{
	UWidgetBlueprintLibrary::SetInputMode_GameOnly(GetOwningPlayer());

	GetOwningPlayer()->SetShowMouseCursor(false);

	DestroyWidget();
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


