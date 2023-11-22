// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnumClass.h"
#include "OnlineSessionSettings.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "StructClass.generated.h"

USTRUCT(Blueprintable, BlueprintType)
struct FMatchDetails
{
	GENERATED_BODY()

public:
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 MaxPlayers;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 StartingMoney;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 TotalRounds;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bBotsAllowed;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsLanMatch;

	FMatchDetails()
	: MaxPlayers(4)
	, StartingMoney(1000)
	, TotalRounds(10)
	, bBotsAllowed(false)
	, bIsLanMatch(false)
	{}
	
};

USTRUCT(Blueprintable, BlueprintType)
struct FPlayerDetails
{
	GENERATED_BODY()

public:
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName PlayerName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int CurrentMoney;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TEnumAsByte<ETeam> Team;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int Score;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Health;
};

USTRUCT(Blueprintable, BlueprintType)
struct FSessionDetails
{
	GENERATED_BODY()

public:
	FOnlineSessionSearchResult Result;

	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString SessionID;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString SessionName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int Latency;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int MaxPlayers;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int CurrentNumberOfPlayers;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int StartingMoney;

	FSessionDetails(): Latency(0), MaxPlayers(8), CurrentNumberOfPlayers(0), StartingMoney(800)
	{
		
	}
};

USTRUCT(Blueprintable, BlueprintType)
struct FLevelAddress
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString IPAddress;
};

USTRUCT(Blueprintable, BlueprintType)
struct FWeaponDetails
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString WeaponName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int WeaponCost;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float WeaponDamage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UTexture2D* WeaponImage;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<class ABaseWeapon> WeaponAsset;

	FWeaponDetails(): WeaponCost(0), WeaponDamage(0), WeaponImage(nullptr)
	{
	}
};

USTRUCT(Blueprintable, BlueprintType)
struct FCharacterDetails
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString CharacterName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TEnumAsByte<ETeam> Team;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class USkeletalMesh* CharacterMesh;
};

USTRUCT(Blueprintable, BlueprintType)
struct FInteractableDetails
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString ActorName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TEnumAsByte<EInteractType> InteractType;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	AActor* ActorReference;

	FInteractableDetails(): ActorName(""), InteractType(PICKUP), ActorReference(nullptr)
	{
	}
};

/*
USTRUCT(Blueprintable, BlueprintType)
struct FFlagDetails
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TEnumAsByte<ETeam> FlagTeam;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TEnumAsByte<ETeam> FlagBaseTeam;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TEnumAsByte<EFlagStatus> FlagStatus;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class AFlagBase* FlagBaseRef;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UStaticMeshComponent* FlagMeshRef;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class AActor* OwningActor;

	FFlagDetails(): FlagTeam(NONE), FlagBaseTeam(), FlagStatus(), FlagBaseRef(nullptr), FlagMeshRef(nullptr),
	                OwningActor(nullptr)
	{
	}
};
*/


UCLASS()
class MULTIPLAYERPROJECT_API UStructClass : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
};
