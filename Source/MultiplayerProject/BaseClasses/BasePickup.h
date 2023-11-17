// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MultiplayerProject/StructClass.h"
#include "MultiplayerProject/InterfaceClasses/InteractableInterface.h"
#include "MultiplayerProject/InterfaceClasses/PickupInterface.h"
#include "BasePickup.generated.h"

UCLASS()
class MULTIPLAYERPROJECT_API ABasePickup : public AActor, public IPickupInterface, public IInteractableInterface
{
	GENERATED_BODY()
	
private:
	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* Actor, UPrimitiveComponent* PrimitiveComponent1, int I, bool bArg, const FHitResult& HitResult);
	UFUNCTION()
	void OnComponentEndOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* Actor, UPrimitiveComponent* PrimitiveComponent1, int I);

public:
	// Sets default values for this actor's properties
	ABasePickup();

	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Flag Components")
	class USceneComponent* mRoot;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Interactable Components")
	class USphereComponent* mRangeComp;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Interactable Properties")
	FInteractableDetails mInteractableDetails;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Interactable Properties")
	TEnumAsByte<EInteractType> InteractableType;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnPlayerOverlapBegin(AActor* Actor, bool isPlayer = false);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnPlayerOverlapEnd(AActor* Actor, bool isPlayer = false);
	
	virtual FInteractableDetails GetInteractableDetails_Implementation() override;;
	virtual void OnPickup_Implementation() override;

};
