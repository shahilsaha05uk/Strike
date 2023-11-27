// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MultiplayerProject/BaseClasses/BaseWidget.h"
#include "OverheadUI.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERPROJECT_API UOverheadUI : public UBaseWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, Category = "Widget")
	class UTextBlock* txtPlayerTeam;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, Category = "Widget")
	class UProgressBar* pbrHealthBar;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void UpdateUI(ETeam Team, const FText& Value);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void UpdateHealth(float Value);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void UpdateName(const FText& Value);
	
};
