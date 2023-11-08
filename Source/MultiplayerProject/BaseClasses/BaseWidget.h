// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MultiplayerProject/EnumClass.h"
#include "BaseWidget.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERPROJECT_API UBaseWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "References")
	TEnumAsByte<EWidgetType> mWidgetType;

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true))
	APlayerController* ControllerRef;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "References")
	class AHUD* mHudRef;

	virtual void NativeConstruct() override;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void DestroyWidget();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void QuitGame();
};
