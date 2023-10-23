// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MultiplayerProject/BaseClasses/BaseWidget.h"
#include "HostServer.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERPROJECT_API UHostServer : public UBaseWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, EditAnywhere, Category = "Widgets")
	class UButton* btnHost;

	UPROPERTY()
	class UMultiplayerSubsystem* MultiplayerPlugin;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SessionProperties")
	TEnumAsByte<enum ETypeOfMatch> MatchType;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SessionProperties")
	int MaxConnections;

	virtual void NativeConstruct() override;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnHost();
	
};
