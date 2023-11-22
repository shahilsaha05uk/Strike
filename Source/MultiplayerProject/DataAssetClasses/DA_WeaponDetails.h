// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MultiplayerProject/EnumClass.h"
#include "MultiplayerProject/StructClass.h"
#include "DA_WeaponDetails.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERPROJECT_API UDA_WeaponDetails : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weapon")
	TEnumAsByte<EWeaponType> WeaponType;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weapon")
	TArray<FWeaponDetails> WeaponDetails;
};
