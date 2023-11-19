// Fill out your copyright notice in the Description page of Project Settings.


#include "MP_PlayerState.h"

#include "MP_HUD.h"
#include "InterfaceClasses/GameInstanceInterface.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"

AMP_PlayerState::AMP_PlayerState()
{
	bReplicates = true;
}

void AMP_PlayerState::BlueprintInitialisation_Implementation(ETeam Team)
{
}

void AMP_PlayerState::InitialisePlayerState_Implementation(ETeam Team)
{
	FMatchDetails MatchDetails;
	UGameInstance* Instance = GetGameInstance();
	if(UKismetSystemLibrary::DoesImplementInterface(Instance, UGameInstance::StaticClass()))
	{
		MatchDetails = IGameInstanceInterface::Execute_GetMatchDetails(Instance);
	}

	mPlayerDetails.Team = Team;
	mPlayerDetails.CurrentMoney = (Team == TERRORIST)? 1000: 2000;

	BlueprintInitialisation(Team);
}


FPlayerDetails AMP_PlayerState::GetPlayerDetails_Implementation()
{
	return mPlayerDetails;
}

void AMP_PlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMP_PlayerState, mPlayerDetails);
}