// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseClasses/BaseInteractable.h"
#include "BaseClasses/BasePickup.h"
#include "GameFramework/Actor.h"
#include "FlagActor.generated.h"

UCLASS()
class MULTIPLAYERPROJECT_API AFlagActor : public ABasePickup
{
	GENERATED_BODY()
	
public:	

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Flag Components")
	UStaticMeshComponent* mFlagBase;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Flag Components")
	UStaticMeshComponent* mFlag;
	
	AFlagActor();

	UFUNCTION(BlueprintCallable)
	void AttachFlag();
	UFUNCTION(BlueprintCallable)
	
	
	virtual void OnPlayerOverlapBegin_Implementation(AActor* Actor, bool isPlayer) override;
	virtual void OnPlayerOverlapEnd_Implementation(AActor* Actor, bool isPlayer) override;
	
};
