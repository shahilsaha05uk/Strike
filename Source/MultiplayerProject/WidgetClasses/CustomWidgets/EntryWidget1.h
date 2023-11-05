// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MultiplayerProject/BaseClasses/BaseWidget.h"
#include "EntryWidget1.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERPROJECT_API UEntryWidget1 : public UBaseWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget), Category = "Widget")
	class UTextBlock* txtLabel;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget), Category = "Widget")
	class UEditableText* txtField;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Private")
	int FieldValue;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Private")
	FText LabelValue;
};
