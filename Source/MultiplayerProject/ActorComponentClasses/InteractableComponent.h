// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractableComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable, BlueprintType )
class MULTIPLAYERPROJECT_API UInteractableComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	UInteractableComponent();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void DropItem(AActor* TargetActor, ACharacter* OwningCharacter);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Interact(AActor* TargetActor, ACharacter* OwningCharacter);
};
