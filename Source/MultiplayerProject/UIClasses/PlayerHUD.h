// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
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

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
	UButton* btnHost;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
	UButton* btnJoin;

	virtual void NativeConstruct() override;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Host();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Join();
	
};
