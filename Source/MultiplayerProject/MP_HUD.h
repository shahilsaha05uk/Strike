// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseClasses/BaseHUD.h"
#include "BaseClasses/BaseWidget.h"
#include "InterfaceClasses/HUDInterface.h"
#include "MP_HUD.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FResumeGameSignature);

UCLASS()
class MULTIPLAYERPROJECT_API AMP_HUD : public ABaseHUD, public IHUDInterface
{
	GENERATED_BODY()

private:

	UPROPERTY()
	TMap<TEnumAsByte<EWidgetType>, UBaseWidget*> mWidgetMap;

	UFUNCTION()
	class UBaseWidget* WidgetCreator(EWidgetType WidgetToSpawn);


	bool WidgetReferenceCheck(UBaseWidget* &WidgetRef, EWidgetType WidgetToSpawn);
	void UpdateWidgetMap(UBaseWidget* Widget, EWidgetType WidgetToUpdate);
	
	UPROPERTY()
	TMap<TEnumAsByte<EWidgetType>, UBaseWidget*> WidgetMap;

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget Classes")
	TSubclassOf<UBaseWidget> PlayerHudClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget Classes")
	TSubclassOf<UBaseWidget> MainMenuClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget Classes")
	TSubclassOf<UBaseWidget> PauseMenuClass;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget Classes")
	TSubclassOf<UBaseWidget> ShopMenuClass;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget Classes")
	TSubclassOf<UBaseWidget> TeamMenuClass;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Delegates")
	FResumeGameSignature ResumeGameSignature;

	
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Init();
	
	virtual UBaseWidget* GetWidget_Implementation(EWidgetType WidgetToGet) override;
	virtual void WidgetDestroyer_Implementation(EWidgetType WidgetToDestroy) override;
	virtual UBaseWidget* WidgetInitialiser_Implementation(EWidgetType WidgetToSpawn) override;
	
};
