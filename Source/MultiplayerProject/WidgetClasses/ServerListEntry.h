// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "MultiplayerProject/StructClass.h"
#include "MultiplayerProject/BaseClasses/BaseWidget.h"
#include "ServerListEntry.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERPROJECT_API UServerListEntry : public UBaseWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

public:

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, EditAnywhere, Category = "Widgets")
	UTextBlock* txtServerName;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, EditAnywhere, Category = "Widgets")
	UTextBlock* txtLatency;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, EditAnywhere, Category = "Widgets")
	UTextBlock* txtPlayerCount;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, EditAnywhere, Category = "Widgets")
	UButton* btnJoin;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Private")
	FSessionDetails mSessionDetails;

	virtual void NativeConstruct() override;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnJoin();
	
	UFUNCTION(BlueprintCallable)
	void UpdateEntry(FSessionDetails SessionDetails);

};
