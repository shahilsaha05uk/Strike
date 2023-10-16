// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "MultiplayerProject/BaseClasses/BaseWidget.h"
#include "OnlineMenu.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERPROJECT_API UOnlineMenu : public UBaseWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;

	UPROPERTY()
	class UMultiplayerSubsystem* MultiplayerPlugin;
	
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, EditAnywhere, Category = "Widgets")
	UButton* btnConnect;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, EditAnywhere, Category = "Widgets")
	UButton* btnHost;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SessionProperties")
	TEnumAsByte<enum ETypeOfMatch> MatchType;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SessionProperties")
	int MaxConnections;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnConnect();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnHost();

};