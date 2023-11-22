// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TextBlock.h"
#include "MultiplayerProject/StructClass.h"
#include "MultiplayerProject/BaseClasses/BaseWidget.h"
#include "PlayerHUD.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERPROJECT_API UPlayerHUD : public UBaseWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, Category = "Widget Properties")
	UTextBlock* txtHealth;
	
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, Category = "Widget Properties")
	UTextBlock* txtMoney;
	
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, Category = "Widget Properties")
	UTextBlock* txtCT;
	
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, Category = "Widget Properties")
	UTextBlock* txtT;
	
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, Category = "Widget Properties")
	UTextBlock* txtAmmo;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Private")
	FMatchDetails mMatchDetails;


	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void UpdateHUD(FPlayerDetails PlayerDetails);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void UpdateScore(int Score, ETeam Team);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void UpdateHealth(float Value);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void UpdateMoney(int Value);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void UpdateAmmo(int Value);
	
};
