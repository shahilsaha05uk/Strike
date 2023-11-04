// Fill out your copyright notice in the Description page of Project Settings.


#include "PauseMenu.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "MultiplayerProject/Multiplayer_GameInstance.h"
#include "MultiplayerProject/SubsystemClasses/MultiplayerSessionsSubsystem.h"

void UPauseMenu::NativeConstruct()
{
	Super::NativeConstruct();

	ResumeGameButton->OnClicked.AddDynamic(this, &ThisClass::OnResumeButtonClick);
	LeaveSessionButton->OnClicked.AddDynamic(this, &ThisClass::OnLeaveSessionButtonClick);
	QuitButton->OnClicked.AddDynamic(this, &ThisClass::QuitGame);

	UGameplayStatics::SetGamePaused(GetWorld(), true);
	GetOwningPlayer()->bShowMouseCursor = true;
	
}

void UPauseMenu::OnResumeButtonClick_Implementation()
{
	DestroyWidget();
}

void UPauseMenu::OnLeaveSessionButtonClick_Implementation()
{
	GetGameInstance()->GetSubsystem<UMultiplayerSessionsSubsystem>()->DestroySession();
}
