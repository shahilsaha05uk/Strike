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


UENUM(Blueprintable, BlueprintType)
enum EWidgetType
{
	PLAYER_HUD,
	MAIN_MENU,
	PAUSE_MENU,
	SHOP_MENU,
	TEAM_MENU,
	FIND_SERVER_MENU,
	HOST_SERVER_MENU
};

UENUM(Blueprintable, BlueprintType)
enum ETeam
{
	TERRORIST,
	COUNTER_TERRORIST
};

UCLASS()
class MULTIPLAYERPROJECT_API UEnumClass : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
};
