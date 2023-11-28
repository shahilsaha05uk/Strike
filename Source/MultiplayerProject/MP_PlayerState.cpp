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
	Kills = 0;
}

void AMP_PlayerState::BeginPlay()
{
	Super::BeginPlay();
	OnPawnSet.AddDynamic(this, &AMP_PlayerState::OnPawnPossessed);
}

void AMP_PlayerState::OnPawnPossessed_Implementation(APlayerState* Player, APawn* Pawn, APawn* OldPawn)
{
	if(OldPawn != nullptr)
	{
		APlayerState* pState = OldPawn->GetPlayerState();
		FPlayerDetails OldPlayerDetails = IPlayerStateInterface::Execute_GetPlayerDetails(pState);
		mPlayerDetails.CurrentMoney = OldPlayerDetails.CurrentMoney;
	}
	
	if(Pawn != nullptr && UKismetSystemLibrary::DoesImplementInterface(Pawn, UPlayerInterface::StaticClass()))
	{
		Pawn->OnTakeAnyDamage.AddDynamic(this, &AMP_PlayerState::OnDamageTaken);
		Health = 100.0f;
		
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

		Health = ((Health - Damage) <= 0)? 0.0f : (Health - Damage);

		if(Health <= 0.0f)
		{
			UpdatePlayerUI(DamagedActor, Health);
			if(UKismetSystemLibrary::DoesImplementInterface(DamagedActor, UPlayerInterface::StaticClass()))
			{
				//TODO: Call the Dead Method from the Damaged Actor
				IPlayerInterface::Execute_Dead(DamagedActor, InstigatedBy);
			}			
			return;
		}
		
		UpdatePlayerUI(DamagedActor, Health);

		IPlayerStateInterface::Execute_UpdateKills(InstigatedBy->PlayerState);
	}
}

void AMP_PlayerState::UpdatePlayerUI_Implementation(AActor* DamagedActor, float HealthValue)
{
	//TODO: Call the Health Bar Update method from the Player
	if(UKismetSystemLibrary::DoesImplementInterface(DamagedActor, UPlayerInterface::StaticClass()))
	{
		IPlayerInterface::Execute_UpdateHealthBar(DamagedActor, HealthValue);
	}

	//TODO: Call the HealthHUD update from the Controller
	if(UKismetSystemLibrary::DoesImplementInterface(DamagedActor->GetOwner(), UControllerInterface::StaticClass()))
	{
		IControllerInterface::Execute_UpdatePlayerHealthUI(DamagedActor->GetOwner(), HealthValue);
	}
}

FPlayerDetails AMP_PlayerState::GetPlayerDetails_Implementation()
{
	return mPlayerDetails;
}

void AMP_PlayerState::UpdateKills_Implementation()
{
	Kills++;
}

void AMP_PlayerState::OnSessionEnd_Implementation(ETeam WinningTeam, int TScore, int CTScore)
{
	
}

void AMP_PlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMP_PlayerState, mPlayerDetails);
	DOREPLIFETIME(AMP_PlayerState, Health);
}


// Initialisation of the Player State

/*
 * the InitialisePlayerState_Implementation() method is called from the Controller On a multicast
 * 
 */

void AMP_PlayerState::Initialise_Implementation(UDA_CharacterMeshDetails* CharacterDetails)
{

}

/*
void AMP_PlayerState::Server_Initialise_Implementation(UDA_CharacterMeshDetails* CharacterDetails)
{
	BlueprintServer_Initialise(CharacterDetails);
}

void AMP_PlayerState::Multicast_Initialise_Implementation(UDA_CharacterMeshDetails* CharacterDetails)
{
	BlueprintMulticast_Initialise(CharacterDetails);
}

void AMP_PlayerState::Client_Initialise_Implementation(UDA_CharacterMeshDetails* CharacterDetails)
{
	BlueprintClient_Initialise(CharacterDetails);
}
*/


/*
void AMP_PlayerState::InitialisePlayerState_Implementation(ETeam Team)
{
	FMatchDetails MatchDetails;
	UGameInstance* Instance = GetGameInstance();
	if(UKismetSystemLibrary::DoesImplementInterface(Instance, UGameInstance::StaticClass()))
	{
		MatchDetails = IGameInstanceInterface::Execute_GetMatchDetails(Instance);
	}

	mPlayerDetails.Team = Team;
	mPlayerDetails.CurrentMoney = MatchDetails.StartingMoney;

	BlueprintInitialisation(Team);
}
*/

