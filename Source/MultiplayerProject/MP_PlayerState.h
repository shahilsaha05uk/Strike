// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnumClass.h"
#include "GameFramework/PlayerState.h"
#include "MP_PlayerState.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERPROJECT_API AMP_PlayerState : public APlayerState
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Player Properties")
	TEnumAsByte<ETeam> mPlayerTeam;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Player Properties")
	FString mUniqueID;

	
	
};
