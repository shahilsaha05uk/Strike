// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MultiplayerProject/EnumClass.h"
#include "DA_CharacterMeshDetails.generated.h"

class APlayerCharacter;
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class MULTIPLAYERPROJECT_API UDA_CharacterMeshDetails : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<APlayerCharacter> PlayerActor;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weapon")
	TEnumAsByte<ETeam> PlayerTeam;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UTexture2D* PlayerImage;
};
