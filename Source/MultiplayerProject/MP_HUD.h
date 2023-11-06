// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseClasses/BaseHUD.h"
#include "BaseClasses/BaseWidget.h"
#include "MP_HUD.generated.h"


UENUM(Blueprintable, BlueprintType)
enum EWidgetType
{
	PLAYER_HUD,
	MAIN_MENU,
	PAUSE_MENU,
	SHOP_MENU
};

UCLASS()
class MULTIPLAYERPROJECT_API AMP_HUD : public ABaseHUD
{
	GENERATED_BODY()

private:

	UPROPERTY()
	TMap<TEnumAsByte<EWidgetType>, UBaseWidget*> mWidgetMap;

	UFUNCTION()
	class UBaseWidget* WidgetCreator(EWidgetType WidgetToSpawn);
	
public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget Classes")
	TSubclassOf<UBaseWidget> PlayerHudClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget Classes")
	TSubclassOf<UBaseWidget> MainMenuClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget Classes")
	TSubclassOf<UBaseWidget> PauseMenuClass;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget Classes")
	TSubclassOf<UBaseWidget> ShopMenuClass;
	

	
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Init();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UBaseWidget* WidgetInitialiser(EWidgetType WidgetToSpawn);
	
};
