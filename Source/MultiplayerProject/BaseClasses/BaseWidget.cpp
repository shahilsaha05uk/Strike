// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWidget.h"

#include "Kismet/KismetSystemLibrary.h"

void UBaseWidget::DestroyWidget_Implementation(UBaseWidget* WidgetToDestroy)
{
	if(WidgetToDestroy == nullptr)
	{
		this->RemoveFromParent();
	}
}

void UBaseWidget::QuitGame_Implementation()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetOwningPlayer(), EQuitPreference::Quit, false);
}

void UBaseWidget::DetachFromParent_Implementation()
{
	this->RemoveFromParent();
}
