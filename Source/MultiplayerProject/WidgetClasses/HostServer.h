// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MultiplayerProject/StructClass.h"
#include "MultiplayerProject/BaseClasses/BaseWidget.h"
#include "HostServer.generated.h"




UCLASS()
class MULTIPLAYERPROJECT_API UHostServer : public UBaseWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, EditAnywhere, Category = "Widgets")
	class UButton* btnHost;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, EditAnywhere, Category = "Widgets")
	class UButton* CloseButton;
	
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, EditAnywhere, Category = "Widgets")
	class UEntryWidget1* TargetScoreEntry; 

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, EditAnywhere, Category = "Widgets")
	class UEntryWidget1* StartingMoneyEntry; 

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SessionProperties")
	FMatchDetails mMatchDetails;

	class ULAN_OnlineSubsystem* mMultiplayerSubsystem;

	virtual void NativeConstruct() override;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnHost();
};
