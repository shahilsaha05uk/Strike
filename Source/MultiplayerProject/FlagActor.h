// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InterfaceClasses/FlagInterface.h"
#include "FlagActor.generated.h"

UCLASS()
class MULTIPLAYERPROJECT_API AFlagActor : public AActor
{
	GENERATED_BODY()

private:
	
public:	

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Flag Components")
	USceneComponent* mRoot;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Flag Components")
	UStaticMeshComponent* mFlag;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Properties")
	TEnumAsByte<ETeam> mTeamFlag;
	
	AFlagActor();
	
};
