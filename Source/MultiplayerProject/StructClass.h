// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnumClass.h"
#include "OnlineSessionSettings.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Sound/SoundCue.h"
#include "StructClass.generated.h"

USTRUCT(Blueprintable, BlueprintType)
struct FMatchDetails
{
	GENERATED_BODY()

public:
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 TargetScore;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 StartingMoney;

	FMatchDetails()
	: TargetScore(0)
	, StartingMoney(200)
	{}
	
};

USTRUCT(Blueprintable, BlueprintType)
struct FPlayerDetails
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText PlayerName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int CurrentMoney;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TEnumAsByte<ETeam> Team;

	// Combat Properties
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int Kills;
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
	float TimePerShot;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int TotalBullets;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float WeaponDamage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	USoundCue* WeaponSound;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UTexture2D* WeaponImage;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<class ABaseWeapon> WeaponAsset;

	FWeaponDetails(): WeaponCost(0), TimePerShot(0), TotalBullets(0), WeaponDamage(0), WeaponSound(nullptr),
	                  WeaponImage(nullptr)
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
	AActor* ActorReference;

	FInteractableDetails(): ActorName(""), ActorReference(nullptr)
	{
	}
};


UCLASS()
class MULTIPLAYERPROJECT_API UStructClass : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
};
