// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MultiplayerProject/EnumClass.h"
#include "MultiplayerProject/TeamInterface.h"
#include "MultiplayerProject/InterfaceClasses/InteractableInterface.h"
#include "BaseInteractable.generated.h"

UCLASS()
class MULTIPLAYERPROJECT_API ABaseInteractable : public AActor, public IInteractableInterface, public ITeamInterface
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Property")
	TEnumAsByte<EInteractableItem> InteractableItem;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Property")
	TEnumAsByte<ETeam> Team;


	virtual void Interact_Implementation(ACharacter* OwnerPlayer) override;
	virtual void DropItem_Implementation(ACharacter* OwnerPlayer) override;
	virtual void AttachToPlayer_Implementation(ACharacter* OwnerPlayer) override;
	virtual void DetachFromPlayer_Implementation(ACharacter* OwnerPlayer) override;

	virtual ETeam GetTeam_Implementation() override;
	virtual EInteractableItem GetInteractableItem_Implementation() override;

};
