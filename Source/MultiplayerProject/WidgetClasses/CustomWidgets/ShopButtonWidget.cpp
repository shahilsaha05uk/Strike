// Fill out your copyright notice in the Description page of Project Settings.


#include "ShopButtonWidget.h"

void UShopButtonWidget::NativeConstruct()
{
	Super::NativeConstruct();

	mButton->OnClicked.AddDynamic(this, &ThisClass::OnClick);
}

void UShopButtonWidget::UpdateList_Implementation()
{
	
}

void UShopButtonWidget::OnClick_Implementation()
{
	//TODO: Call a delegate to deactivate the last buttons
	
	//TODO: Activate the buttons related to this
	
}
