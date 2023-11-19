// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUD.h"

void UPlayerHUD::UpdateHealth(float Value)
{
	UE_LOG(LogTemp, Warning, TEXT("Player HUD has updated the Health"));

	txtHealth->SetText(FText::AsNumber(Value));
}
