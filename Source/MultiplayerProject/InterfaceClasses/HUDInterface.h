// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MultiplayerProject/EnumClass.h"
#include "MultiplayerProject/WidgetClasses/PlayerHUD.h"
#include "UObject/Interface.h"
#include "HUDInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UHUDInterface : public UInterface
{
	GENERATED_BODY()
};


class MULTIPLAYERPROJECT_API IHUDInterface
{
	GENERATED_BODY()


public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	class UBaseWidget* WidgetInitialiser(EWidgetType WidgetToSpawn);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void WidgetDestroyer(EWidgetType WidgetToDestroy);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	class UBaseWidget* GetWidget(EWidgetType WidgetToGet);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	UPlayerHUD* OnPawnSpawn();

};
