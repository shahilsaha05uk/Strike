// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MultiplayerProject/BaseClasses/BaseWidget.h"
#include "MultiplayerProject/DataAssetClasses/DA_CharacterMeshDetails.h"

#include "TeamChooseUI.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERPROJECT_API UTeamChooseUI : public UBaseWidget
{
	GENERATED_BODY()

private:

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
	class UButton* btnCounterTerrorist;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
	class UButton* btnTerrorist;

	virtual void NativeConstruct() override;

	virtual void InitialiseWidget(AMP_HUD* Hud, AInputController* InputController) override;

	UFUNCTION(BlueprintCallable)
	void OnDecisionMade();


	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnCounterTerroristButtonCLick();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnTerroristButtonCLick();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "References")
	TArray<UDA_CharacterMeshDetails*> TerroristArray;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "References")
	TArray<UDA_CharacterMeshDetails*> CounterTerroristArray;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "References")
	class AMP_PlayerState* PlayerStateRef;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "References")
	class UMultiplayer_GameInstance* GameInstance;

	
};
