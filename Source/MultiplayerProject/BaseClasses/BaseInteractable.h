// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "MultiplayerProject/StructClass.h"
#include "MultiplayerProject/InterfaceClasses/PickupInterface.h"
#include "BaseInteractable.generated.h"

UCLASS()
class MULTIPLAYERPROJECT_API ABaseInteractable : public AActor, public IPickupInterface
{
	GENERATED_BODY()

private:
	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* Actor, UPrimitiveComponent* PrimitiveComponent1, int I, bool bArg, const FHitResult& HitResult);
	UFUNCTION()
	void OnComponentEndOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* Actor, UPrimitiveComponent* PrimitiveComponent1, int I);

public:

	ABaseInteractable();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Flag Components")
	USceneComponent* mRoot;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Interactable Components")
	USphereComponent* mRangeComp;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Interactable Properties")
	FFocusedActorDetails ActorDetails;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Interactable Properties")
	TEnumAsByte<EInteractType> InteractableType;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnPlayerOverlapBegin(AActor* Actor, bool isPlayer = false);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnPlayerOverlapEnd(AActor* Actor, bool isPlayer = false);

	virtual void OnInteract_Implementation() override;
	virtual EInteractType GetInteractType_Implementation() override;

};
