// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MultiplayerProject/StructClass.h"
#include "MultiplayerProject/BaseClasses/BaseWidget.h"
#include "BuyMenu.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdateListSignature, UDA_WeaponDetails*, WeaponDetails);
UCLASS()
class MULTIPLAYERPROJECT_API UBuyMenu : public UBaseWidget
{
	GENERATED_BODY()

public:
	
	UPROPERTY(meta = (BindWidget), EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	class UVerticalBox* mMenuList;
	
	UPROPERTY(meta = (BindWidget), EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	class UButton* CloseButton;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, BlueprintReadWrite)
	FOnUpdateListSignature OnUpdateList;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Private")
	TArray<class UDA_WeaponDetails*> WeaponArray;

	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ClearList();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void UpdateList(UDA_WeaponDetails* WeaponDetails);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnBack();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void GoToMainMenu();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SpawnWeapon(FWeaponDetails WeaponDetails);
};
