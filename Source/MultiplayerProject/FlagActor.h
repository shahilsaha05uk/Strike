// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseClasses/BaseInteractable.h"
#include "BaseClasses/BasePickup.h"
#include "GameFramework/Actor.h"
#include "InterfaceClasses/FlagInterface.h"
#include "FlagActor.generated.h"

UCLASS()
class MULTIPLAYERPROJECT_API AFlagActor : public AActor, public IFlagInterface
{
	GENERATED_BODY()
	
public:	

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Flag Components")
	UStaticMeshComponent* mFlagBase;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Flag Components")
	USceneComponent* mFlagParent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Flag Components")
	UStaticMeshComponent* mFlag;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Flag Components")
	class USceneComponent* mRoot;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Flag Components")
	class UWidgetComponent* mFlagWidget;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Interactable Components")
	class USphereComponent* mRangeComp;
	
	AFlagActor();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnComponentBeginOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* Actor, UPrimitiveComponent* PrimitiveComponent1, int I, bool bArg, const FHitResult& HitResult);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnComponentEndOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* Actor, UPrimitiveComponent* PrimitiveComponent1, int I);


	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnFlagCaptured(USkeletalMeshComponent* MeshToAttachTo, FName SocketName);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnFlagRetrieved();

	virtual void CaptureFlag_Implementation(USkeletalMeshComponent* MeshToAttachTo, FName SocketName) override;
	virtual void RetrieveFlag_Implementation() override;
};
