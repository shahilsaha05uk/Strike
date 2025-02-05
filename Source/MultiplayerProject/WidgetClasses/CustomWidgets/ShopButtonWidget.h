// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "MultiplayerProject/StructClass.h"
#include "MultiplayerProject/BaseClasses/BaseWidgetClasses/BaseButton.h"

#include "ShopButtonWidget.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERPROJECT_API UShopButtonWidget : public UBaseButton
{
	GENERATED_BODY()

public:

	UPROPERTY(meta = (BindWidget), EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	UButton* mButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets", meta = (ExposeOnSpawn))
	class UDA_WeaponDetails* WeaponDetails;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References", meta = (ExposeOnSpawn))
	class UBuyMenu* ParentRef;

	virtual void NativeConstruct() override;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnClick();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void UpdateList();
};
