// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu_GameMode.h"

#include "OnlineSubsystem.h"

AMainMenu_GameMode::AMainMenu_GameMode()
{
	OnPlayerSpawnSignature.AddDynamic(this, &ThisClass::OnPlayerSpawn);
}

void AMainMenu_GameMode::OnPlayerSpawn_Implementation(APlayerCharacter* PlayerRef)
{
	
}
