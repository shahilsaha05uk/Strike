// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MultiplayerProject/EnumClass.h"
#include "BaseWidget.generated.h"

UCLASS()
class MULTIPLAYERPROJECT_API UBaseWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "References")
	TEnumAsByte<EWidgetType> mWidgetType;


public:
	
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true))
	class APlayerController* ControllerRef;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true))
	class AInputController* mInputController;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "References")
	class AHUD* mHudRef;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "References")
	class AMP_HUD* mHUD;
	
	virtual void NativeConstruct() override;
	virtual void InitialiseWidget(class AMP_HUD* Hud = nullptr, AInputController* InputController = nullptr);

	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void DestroyWidget();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void QuitGame();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ResumeGame();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void PauseGame(bool Value);
};
