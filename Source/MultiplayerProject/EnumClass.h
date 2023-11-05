// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "EnumClass.generated.h"

UENUM(Blueprintable, BlueprintType)
enum EWeaponType
{
	Primary,
	Secondary,
	Equipment
};



UCLASS()
class MULTIPLAYERPROJECT_API UEnumClass : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
};
