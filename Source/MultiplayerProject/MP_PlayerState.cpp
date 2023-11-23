// Fill out your copyright notice in the Description page of Project Settings.


#include "MP_PlayerState.h"

#include "MP_HUD.h"
#include "InterfaceClasses/ControllerInterface.h"
#include "InterfaceClasses/GameInstanceInterface.h"
#include "InterfaceClasses/PlayerInterface.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"

AMP_PlayerState::AMP_PlayerState()
{
	bReplicates = true;
}

void AMP_PlayerState::BeginPlay()
{
	Super::BeginPlay();
	OnPawnSet.AddDynamic(this, &AMP_PlayerState::OnPawnPossessed);

}

void AMP_PlayerState::OnPawnPossessed_Implementation(APlayerState* Player, APawn* Pawn, APawn* OldPawn)
{
	if(Pawn != nullptr && UKismetSystemLibrary::DoesImplementInterface(Pawn, UPlayerInterface::StaticClass()))
	{
		Pawn->OnTakeAnyDamage.AddDynamic(this, &AMP_PlayerState::OnDamageTaken);
	}
}

void AMP_PlayerState::OnDamageTaken_Implementation(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	AController* InstigatedBy, AActor* DamageCauser)
{
	APlayerState* InstigatorPState = InstigatedBy->PlayerState;
	APlayerState* DamagedActorPState = this;
	
	if(UKismetSystemLibrary::DoesImplementInterface(InstigatorPState, UPlayerStateInterface::StaticClass()))
	{
		FPlayerDetails InstigatorPlayerDetails = IPlayerStateInterface::Execute_GetPlayerDetails(InstigatorPState);
		FPlayerDetails DamagedActorPlayerDetails = IPlayerStateInterface::Execute_GetPlayerDetails(DamagedActorPState);

		if(DamagedActorPlayerDetails.Team == InstigatorPlayerDetails.Team) return;

		Health -= Damage;

		UpdatePlayerUI(DamagedActor, Health);

	}
}

void AMP_PlayerState::UpdatePlayerUI_Implementation(AActor* DamagedActor, float Damage)
{
	if(Health <= 0.0f)
	{
		if(UKismetSystemLibrary::DoesImplementInterface(DamagedActor->GetOwner(), UPlayerInterface::StaticClass()))
		{
			//TODO: Call the Dead Method from the Damaged Actor
		}
	}
	else
	{
		//TODO: Call the Health Bar Update method from the Player
		if(UKismetSystemLibrary::DoesImplementInterface(DamagedActor, UPlayerInterface::StaticClass()))
		{
			IPlayerInterface::Execute_UpdateHealthBar(DamagedActor, Health);
		}

		//TODO: Call the HealthHUD update from the Controller
		if(UKismetSystemLibrary::DoesImplementInterface(DamagedActor->GetOwner(), UControllerInterface::StaticClass()))
		{
			IControllerInterface::Execute_UpdatePlayerHealthUI(DamagedActor->GetOwner(), Health);
		}
	}
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
	DOREPLIFETIME(AMP_PlayerState, Health);
}

