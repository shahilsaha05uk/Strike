// Fill out your copyright notice in the Description page of Project Settings.


#include "BuyMenu.h"

#include "Components/Button.h"

void UBuyMenu::NativeConstruct()
{
	Super::NativeConstruct();

	OnUpdateList.AddDynamic(this, &ThisClass::UpdateList);
	CloseButton->OnClicked.AddDynamic(this, &ThisClass::OnClose);
}

void UBuyMenu::GoToMainMenu_Implementation()
{
	
}

void UBuyMenu::UpdateList_Implementation(UDA_WeaponDetails* WeaponDetails)
{
	
}

void UBuyMenu::OnClose_Implementation()
{
	DestroyWidget();
}

void UBuyMenu::OnBack_Implementation()
{
	
}

/*void UBuyMenu::ClearList_Implementation()
{
	
}*/