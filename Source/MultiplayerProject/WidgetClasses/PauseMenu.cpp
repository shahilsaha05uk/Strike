// Fill out your copyright notice in the Description page of Project Settings.


#include "PauseMenu.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Button.h"
#include "MultiplayerProject/SubsystemClasses/LAN_OnlineSubsystem.h"

void UPauseMenu::NativeConstruct()
{
	Super::NativeConstruct();

	ResumeGameButton->OnClicked.AddDynamic(this, &ThisClass::ResumeGame);
	LeaveSessionButton->OnClicked.AddDynamic(this, &ThisClass::OnLeaveSessionButtonClick);
	QuitButton->OnClicked.AddDynamic(this, &ThisClass::QuitGame);

	GetOwningPlayer()->SetShowMouseCursor(true);
	UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(GetOwningPlayer());

}

void UPauseMenu::OnResumeButtonClick_Implementation()
{
	//ResumeGame();
}

void UPauseMenu::OnLeaveSessionButtonClick_Implementation()
{
	GetGameInstance()->GetSubsystem<ULAN_OnlineSubsystem>()->DestroySession();
}
