// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWidget.h"

void UBaseWidget::DestroyWidget_Implementation(UBaseWidget* WidgetToDestroy)
{
	if(WidgetToDestroy == nullptr)
	{
		this->RemoveFromParent();
	}
}
