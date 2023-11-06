// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "Engine/DataAsset.h"
#include "DA_UIInputs.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERPROJECT_API UDA_UIInputs : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI Inputs")
	UInputAction* IA_Pause;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI Inputs")
	UInputAction* IA_Shop;
	
};
