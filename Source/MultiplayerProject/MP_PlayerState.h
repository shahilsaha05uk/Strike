// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnumClass.h"
#include "StructClass.h"
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

	AMP_PlayerState();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void BlueprintInitialisation(ETeam Team);
	
	virtual void InitialisePlayerState_Implementation(ETeam Team) override;
	virtual FPlayerDetails GetPlayerDetails_Implementation() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

};
