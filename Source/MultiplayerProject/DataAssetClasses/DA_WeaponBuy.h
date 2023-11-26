// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DA_WeaponDetails.h"
#include "Engine/DataAsset.h"
#include "MultiplayerProject/EnumClass.h"
#include "DA_WeaponBuy.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class MULTIPLAYERPROJECT_API UDA_WeaponBuy : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TEnumAsByte<EWeaponType> WeaponType;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<UDA_WeaponDetails*> WeaponArray;
};
