// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnumClass.h"
#include "InterfaceClasses/FlagInterface.h"
#include "FlagBase.generated.h"

UCLASS()
class MULTIPLAYERPROJECT_API AFlagBase : public AActor, public IFlagInterface
{
	GENERATED_BODY()

private:

	UPROPERTY()
	TEnumAsByte<EFlagStatus> mFlagStatus;

	
	
public:	

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Flag Components")
	UStaticMeshComponent* mFlagBase;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Interactable Components")
	class USphereComponent* mRangeComp;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Flag Components")
	class USceneComponent* mRoot;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Flag Components")
	class UWidgetComponent* mFlagWidget;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Flag Components")
	UChildActorComponent* mFlagComponent;


	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Properties")
	TEnumAsByte<ETeam> mTeamBase;
	
	AFlagBase();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnComponentBeginOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* Actor, UPrimitiveComponent* PrimitiveComponent1, int I, bool bArg, const FHitResult& HitResult);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnComponentEndOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* Actor, UPrimitiveComponent* PrimitiveComponent1, int I);

	virtual void CaptureFlag_Implementation(USkeletalMeshComponent* MeshToAttachTo, FName SocketName) override;
	virtual void RetrieveFlag_Implementation() override;
	virtual EFlagStatus GetFlagStatus_Implementation() override;
	virtual void SetFlagStatus_Implementation(EFlagStatus UpdatedStatus) override;
	virtual void DropFlag_Implementation() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnFlagCaptured(USkeletalMeshComponent* MeshToAttachTo, FName SocketName);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnFlagRetrieved();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnFlagDrop();


};
