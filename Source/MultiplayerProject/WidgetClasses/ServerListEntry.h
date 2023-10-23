// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "MultiplayerProject/BaseClasses/BaseWidget.h"
#include "ServerListEntry.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERPROJECT_API UServerListEntry : public UBaseWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

public:

	/*
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	virtual void NativeOnItemSelectionChanged(bool bIsSelected) override;
	virtual void NativeOnItemExpansionChanged(bool bIsExpanded) override;
	virtual void NativeOnEntryReleased() override;
	*/


	
};
