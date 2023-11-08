// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MultiplayerProject/BaseClasses/BaseWidget.h"
#include "FindServer.generated.h"

struct FSessionDetails;
/**
 * 
 */
UCLASS()
class MULTIPLAYERPROJECT_API UFindServer : public UBaseWidget
{
	GENERATED_BODY()

public:
	
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, EditAnywhere, Category = "Widgets")
	class UButton* btnRefresh;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, EditAnywhere, Category = "Widgets")
	class UButton* CloseButton;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, EditAnywhere, Category = "Widgets")
	class UListView* mServerList;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SessionProperties")
	TArray<FSessionDetails> mSessionDetails;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Private")
	class ULAN_OnlineSubsystem* MultiplayerSessionsSubsystem;

	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnRefresh();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void UpdateList();

private:
	UFUNCTION()
	void OnFindSessionComplete(TArray<FSessionDetails> SessionDetails);

	
};
