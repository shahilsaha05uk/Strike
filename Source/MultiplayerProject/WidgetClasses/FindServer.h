// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MultiplayerProject/BaseClasses/BaseWidget.h"
#include "MultiplayerProject/Session_GameInstanceComponent.h"
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
	TArray<FSessionDetails> mSessionDetails;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SessionProperties")
	int MaxConnections;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SessionProperties")
	USession_GameInstanceComponent* mSessionComp;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, EditAnywhere, Category = "Widgets")
	class UButton* btnConnect;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, EditAnywhere, Category = "Widgets")
	class UListView* mServerList;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnFindSessionComplete(const TArray<FSessionDetails>& OnlineSessionSearchResults, bool bSuccessful);

	virtual void NativeConstruct() override;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnConnect();

	
};
