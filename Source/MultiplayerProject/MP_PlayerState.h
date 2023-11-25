// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnumClass.h"
#include "StructClass.h"
#include "DataAssetClasses/DA_CharacterMeshDetails.h"
#include "GameFramework/PlayerState.h"
#include "InterfaceClasses/PlayerStateInterface.h"
#include "MP_PlayerState.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERPROJECT_API AMP_PlayerState : public APlayerState, public IPlayerStateInterface
{
	GENERATED_BODY()

public:

	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere, Category = "Player Properties")
	FPlayerDetails mPlayerDetails;

	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere)
	int Kills;
	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere)
	float Health;

	AMP_PlayerState();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnPawnPossessed(APlayerState* Player, APawn* Pawn, APawn* OldPawn);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnDamageTaken(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION(Client, Reliable, BlueprintCallable)
	void UpdatePlayerUI(AActor* DamagedActor, float HealthValue);
	
	
	virtual void Initialise_Implementation(UDA_CharacterMeshDetails* CharacterDetails) override;

	/*
	UFUNCTION(Server, Reliable)
	void Server_Initialise(UDA_CharacterMeshDetails* CharacterDetails);
	UFUNCTION(Client, Reliable)
	void Client_Initialise(UDA_CharacterMeshDetails* CharacterDetails);
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_Initialise(UDA_CharacterMeshDetails* CharacterDetails);
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BlueprintServer_Initialise(UDA_CharacterMeshDetails* CharacterDetails);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BlueprintClient_Initialise(UDA_CharacterMeshDetails* CharacterDetails);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BlueprintMulticast_Initialise(UDA_CharacterMeshDetails* CharacterDetails);
	*/



	virtual FPlayerDetails GetPlayerDetails_Implementation() override;
	virtual void UpdateKills_Implementation() override;

	virtual void OnSessionEnd_Implementation(ETeam WinningTeam, int TScore, int CTScore) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
