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
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Private")
	FMatchDetails mMatchDetails;


	UFUNCTION(BlueprintCallable)
	void UpdateHealth(float Value);
	
};
