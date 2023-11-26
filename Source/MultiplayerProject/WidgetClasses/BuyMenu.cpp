// Fill out your copyright notice in the Description page of Project Settings.


#include "BuyMenu.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Button.h"

void UBuyMenu::NativeConstruct()
{
	Super::NativeConstruct();

	OnUpdateList.AddDynamic(this, &ThisClass::UpdateList);
	
	CloseButton->OnClicked.AddDynamic(this, &ThisClass::ResumeGame);

	GetOwningPlayer()->SetShowMouseCursor(true);
	
	UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(GetOwningPlayer());
}

void UBuyMenu::SpawnWeapon_Implementation(FWeaponDetails WeaponDetails)
{
	
}

void UBuyMenu::GoToMainMenu_Implementation()
{
	
}

void UBuyMenu::UpdateList_Implementation(UDA_WeaponBuy* WeaponDetails)
{
	
}

void UBuyMenu::OnBack_Implementation()
{
	
}

void UBuyMenu::ClearList_Implementation()
{
	
}