// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "Engine/DataAsset.h"
#include "DA_InputData.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class MULTIPLAYERPROJECT_API UDA_InputData : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Input Data")
	UInputAction* IA_Move;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Input Data")
	UInputAction* IA_Look;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Input Data")
	UInputAction* IA_Jump;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Input Data")
	UInputAction* IA_Crouch;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Input Data")
	UInputAction* IA_Aim;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Input Data")
	UInputAction* IA_Shoot;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Input Data")
	UInputAction* IA_Pickup;
};
