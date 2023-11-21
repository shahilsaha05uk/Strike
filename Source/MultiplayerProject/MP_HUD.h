// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseClasses/BaseHUD.h"
#include "BaseClasses/BaseWidget.h"
#include "DataAssetClasses/DA_CharacterMeshDetails.h"
#include "InterfaceClasses/HUDInterface.h"
#include "MP_HUD.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FResumeGameSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTeamChosenSignature, UDA_CharacterMeshDetails*, CharacterDetails);

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
	TSubclassOf<UBaseWidget> HostServerMenuClass;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget Classes")
	TSubclassOf<UBaseWidget> FindServerMenuClass;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget Classes")
	TSubclassOf<UBaseWidget> TeamMenuClass;

#pragma region Delegates
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Delegates")
	FResumeGameSignature ResumeGameSignature;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnTeamChosenSignature OnTeamChosen;

#pragma region endregion

#pragma region Team UI

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnDecisionMade(UDA_CharacterMeshDetails* CharacterDetails);

#pragma endregion
	
	
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Init();
	
	virtual UBaseWidget* GetWidget_Implementation(EWidgetType WidgetToGet) override;
	virtual void WidgetDestroyer_Implementation(EWidgetType WidgetToDestroy) override;
	virtual UBaseWidget* WidgetInitialiser_Implementation(EWidgetType WidgetToSpawn) override;

	


};
