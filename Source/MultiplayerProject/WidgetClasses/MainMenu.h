// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MultiplayerProject/BaseClasses/BaseWidget.h"
#include "MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERPROJECT_API UMainMenu : public UBaseWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget), Category = "Widget Properties")
	class UButton* HostGameButton;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget), Category = "Widget Properties")
	class UButton* ServerBrowserButton;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget), Category = "Widget Properties")
	class UButton* SettingsButton;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget), Category = "Widget Properties")
	class UButton* GameInfoButton;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget), Category = "Widget Properties")
	class UButton* QuitButton;

	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Button Clicked Methods")
	void OnHostGameButtonClick();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Button Clicked Methods")
	void OnServerBrowserButtonClick();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Button Clicked Methods")
	void OnSettingsButtonClick();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Button Clicked Methods")
	void OnGameInfoButtonClick();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Button Clicked Methods")
	void OnQuitButtonClick();

	

};
