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
enum EInteractableItem
{
	FLAG,
	WEAPON,
};

UENUM(Blueprintable, BlueprintType)
enum EInteractAction
{
	INTERACT,
	DROP,
};

UENUM(Blueprintable, BlueprintType)
enum EHUDValue
{
	AMMO_Val,
	MONEY_Val,
	HEALTH_Val,
	TARGET_Val,
	T_SCORE_Val,
	CT_SCORE_Val,
};



UCLASS()
class MULTIPLAYERPROJECT_API UEnumClass : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
};
