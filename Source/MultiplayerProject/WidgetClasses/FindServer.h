// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MultiplayerProject/BaseClasses/BaseWidget.h"
#include "FindServer.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERPROJECT_API UFindServer : public UBaseWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SessionProperties")
	int MaxConnections;

	UPROPERTY()
	class UMultiplayerSubsystem* MultiplayerPlugin;
	
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, EditAnywhere, Category = "Widgets")
	class UButton* btnConnect;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, EditAnywhere, Category = "Widgets")
	class UListView* mServerList;
	
	
	virtual void NativeConstruct() override;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnConnect();

	
};
