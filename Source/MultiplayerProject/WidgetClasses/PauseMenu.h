// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MultiplayerProject/BaseClasses/BaseWidget.h"
#include "PauseMenu.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERPROJECT_API UPauseMenu : public UBaseWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, EditAnywhere, Category = "Widgets")
	class UButton* ResumeGameButton;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, EditAnywhere, Category = "Widgets")
	class UButton* LeaveSessionButton;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, EditAnywhere, Category = "Widgets")
	class UButton* QuitButton;

	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnResumeButtonClick();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnLeaveSessionButtonClick();
	
};
