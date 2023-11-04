// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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

};

USTRUCT(Blueprintable, BlueprintType)
struct FLevelAddress
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString IPAddress;
};


UCLASS()
class MULTIPLAYERPROJECT_API UStructClass : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
};
