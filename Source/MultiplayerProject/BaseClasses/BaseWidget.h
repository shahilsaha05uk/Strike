// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BaseWidget.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERPROJECT_API UBaseWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true))
	APlayerController* ControllerRef;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void DetachFromParent();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void DestroyWidget(UBaseWidget* WidgetToDestroy = nullptr);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void QuitGame();
};
