// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MultiplayerProject/StructClass.h"
#include "MultiplayerProject/BaseClasses/BaseWidgetClasses/BaseButton.h"

#include "ShopItemButton.generated.h"

class UButton;
/**
 * 
 */
UCLASS()
class MULTIPLAYERPROJECT_API UShopItemButton : public UBaseButton
{
	GENERATED_BODY()

public:

	UPROPERTY(meta = (BindWidget), EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	UButton* mButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets", meta = (ExposeOnSpawn))
	FWeaponDetails WeaponDetails;

	virtual void NativeConstruct() override;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnClick();
	
};
