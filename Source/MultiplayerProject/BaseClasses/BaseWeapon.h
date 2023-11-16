// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MultiplayerProject/StructClass.h"
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
	class AActor* mOwnerRef;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "References")
	bool bIsFiring;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Private")
	FWeaponDetails mWeaponDetails;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, BlueprintAssignable, Category = "Private")
	FStartShootingSignature StartShootingSignature;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, BlueprintAssignable, Category = "Private")
	FStopShootingSignature StopShootingSignature;

	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Init();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnComponentBeginOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* Actor, UPrimitiveComponent* PrimitiveComponent1, int I, bool bArg, const FHitResult& HitResult);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnComponentEndOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* Actor, UPrimitiveComponent* PrimitiveComponent1, int I);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void PlayWeaponSound();


public:

	// Server Methods

	// Fire
	UFUNCTION(Server, Unreliable, BlueprintCallable)
	void Server_Fire();
	UFUNCTION(BlueprintNativeEvent)
	void Blueprint_Server_Fire();

	// Stop Fire
	UFUNCTION(Server, Unreliable, BlueprintCallable)
	void Server_StopFire();
	UFUNCTION(BlueprintNativeEvent)
	void Blueprint_ServerStopFire();

	// Equip Weapon
	UFUNCTION(Server, Unreliable, BlueprintCallable)
	void Server_Equip();
	UFUNCTION(BlueprintNativeEvent)
	void Blueprint_Server_EquipWeapon();
	
public:

	// Multicast Methods

	// Fire
	UFUNCTION(NetMulticast, Unreliable, BlueprintCallable)
	void Multicast_Fire();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Blueprint_Multicast_Fire();

	//Stop Fire
	UFUNCTION(NetMulticast, Unreliable, BlueprintCallable)
	void Multicast_StopFire();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Blueprint_Multicast_StopFire();

	// Equip Weapon
	UFUNCTION(NetMulticast, Unreliable, BlueprintCallable)
	void Multicast_EquipWeapon();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Blueprint_Multicast_EquipWeapon();

public:

	// Server Method Callers
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Fire();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void StopFire();

	virtual void OnInteract_Implementation() override;

	virtual EInteractType GetInteractType_Implementation() override;
};