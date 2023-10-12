// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUD.h"


void UPlayerHUD::NativeConstruct()
{
	btnHost->OnClicked.AddDynamic(this, &UPlayerHUD::Host);
	btnJoin->OnClicked.AddDynamic(this, &UPlayerHUD::Join);
	
	Super::NativeConstruct();
}

void UPlayerHUD::Host_Implementation()
{
	
}

void UPlayerHUD::Join_Implementation()
{
	
}

