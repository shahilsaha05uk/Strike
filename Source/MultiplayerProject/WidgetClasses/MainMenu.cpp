// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "Components/Button.h"


void UMainMenu::NativeConstruct()
{
	Super::NativeConstruct();
	
	HostGameButton->OnClicked.AddDynamic(this, &UMainMenu::OnHostGameButtonClick);
	ServerBrowserButton->OnClicked.AddDynamic(this, &UMainMenu::OnServerBrowserButtonClick);
	SettingsButton->OnClicked.AddDynamic(this, &UMainMenu::OnSettingsButtonClick);
	QuitButton->OnClicked.AddDynamic(this, &UMainMenu::OnQuitButtonClick);
}

void UMainMenu::OnHostGameButtonClick_Implementation()
{
}

void UMainMenu::OnServerBrowserButtonClick_Implementation()
{
}

void UMainMenu::OnSettingsButtonClick_Implementation()
{
}

void UMainMenu::OnQuitButtonClick_Implementation()
{
}
