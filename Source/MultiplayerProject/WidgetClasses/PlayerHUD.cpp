// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUD.h"

void UPlayerHUD::UpdateHealth(float Value)
{
	txtHealth->SetText(FText::AsNumber(Value));
}

void UPlayerHUD::UpdateMoney(int Value)
{
	txtMoney->SetText(FText::AsNumber(Value));
}
