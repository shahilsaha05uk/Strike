// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "EnumClass.generated.h"

UENUM(Blueprintable, BlueprintType)
enum EWeaponType
{
	None,
	Primary,
	Secondary,
	Ammo,
	MediKit
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
	HOST_SERVER_MENU,
	SCOREBOARD
};

UENUM(Blueprintable, BlueprintType)
enum ETeam
{
	NONE,
	TERRORIST,
	COUNTER_TERRORIST
};

UENUM(Blueprintable, BlueprintType)
enum EFlag
{
	CT_FLAG,
	T_FLAG
};

UENUM(Blueprintable, BlueprintType)
enum EInteractType
{
	EQUIPPABLE,
	PICKUP,
	COLLECTIBLE
};

UENUM(Blueprintable, BlueprintType)
enum EFlagStatus
{
	IN_ORIGINAL_PLACE,
	CAPTURED,
	RETRIEVED,
	DROPPED,
	IN_OPPONENT_BASE
};




UCLASS()
class MULTIPLAYERPROJECT_API UEnumClass : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
};
