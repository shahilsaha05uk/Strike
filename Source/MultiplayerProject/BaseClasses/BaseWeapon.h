// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MultiplayerProject/InterfaceClasses/PickupInterface.h"
#include "BaseWeapon.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStartShootingSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStopShootingSignature);

UCLASS()
class MULTIPLAYERPROJECT_API ABaseWeapon : public AActor, public IPickupInterface
{
	GENERATED_BODY()
	
public:
	ABaseWeapon();
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Actor Components")
	class USceneComponent* mProjectileSpawnLocation;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Actor Components")
	class USphereComponent* mCollisionComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Actor Components")
	class UWidgetComponent* mUIComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Actor Components")
	TSubclassOf<UUserWidget> mWidgetClass;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "References")
	USoundBase* mSoundToPlay;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "References")
	FName WeaponSocket;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "References")
	class AActor* mOwnerRef;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "References")
	bool bIsFiring;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, BlueprintAssignable, Category = "Private")
	FStartShootingSignature StartShootingSignature;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, BlueprintAssignable, Category = "Private")
	FStopShootingSignature StopShootingSignature;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnComponentBeginOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* Actor, UPrimitiveComponent* PrimitiveComponent1, int I, bool bArg, const FHitResult& HitResult);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnComponentEndOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* Actor, UPrimitiveComponent* PrimitiveComponent1, int I);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void StopFire();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void PlayWeaponSound();

	virtual ABaseWeapon* EquipWeapon_Implementation() override;

/*
public:

	// Client RPC
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void Client_Fire();
*/

public:

	// Server RPC
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ServerFire();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void MultiClientFire();
	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void Fire();

private:
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void NativeServer_Fire();
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
	void NativeMultiClient_Fire();
	


};