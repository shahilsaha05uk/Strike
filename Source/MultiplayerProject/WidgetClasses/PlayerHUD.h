// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TextBlock.h"
#include "MultiplayerProject/StructClass.h"
#include "MultiplayerProject/BaseClasses/BaseWidget.h"
#include "MultiplayerProject/InterfaceClasses/PlayerHUDInterface.h"
#include "PlayerHUD.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERPROJECT_API UPlayerHUD : public UBaseWidget, public IPlayerHUDInterface
{
	GENERATED_BODY()

public:

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, Category = "Widget Properties")
	UTextBlock* txtHealth;
	
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, Category = "Widget Properties")
	UTextBlock* txtMoney;
	
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, Category = "Widget Properties")
	UTextBlock* txtTarget;
	
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, Category = "Widget Properties")
	UTextBlock* txtCT;
	
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, Category = "Widget Properties")
	UTextBlock* txtT;
	
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, Category = "Widget Properties")
	UTextBlock* txtAmmo;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Private")
	FMatchDetails mMatchDetails;


	virtual void UpdateHUD_Implementation(FPlayerDetails PlayerDetails) override;

	virtual void UpdateTarget_Implementation(int Target) override;
	virtual void UpdateAmmo_Implementation(int Value) override;
	virtual void UpdateHealth_Implementation(float Value) override;
	virtual void UpdateMoney_Implementation(int Value) override;
	virtual void UpdateScore_Implementation(int Score, ETeam Team) override;
};
