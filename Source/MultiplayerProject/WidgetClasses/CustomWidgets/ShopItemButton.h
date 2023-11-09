// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MultiplayerProject/StructClass.h"
#include "MultiplayerProject/BaseClasses/BaseWidgetClasses/BaseButton.h"

#include "ShopItemButton.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSpawnWeaponSignature, FWeaponDetails, WeaponDetails);

UCLASS()
class MULTIPLAYERPROJECT_API UShopItemButton : public UBaseButton
{
	GENERATED_BODY()

public:

	UPROPERTY(meta = (BindWidget), EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	class UButton* mButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets", meta = (ExposeOnSpawn))
	FWeaponDetails WeaponDetails;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References", meta = (ExposeOnSpawn))
	UBaseWidget* ParentRef;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, BlueprintReadWrite)
	FOnSpawnWeaponSignature OnSpawnWeaponSignature;

	
	virtual void NativeConstruct() override;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnClick();
	
};
