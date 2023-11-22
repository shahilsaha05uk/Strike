// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MultiplayerProject/StructClass.h"
#include "MultiplayerProject/InterfaceClasses/InteractableInterface.h"
#include "BaseWeapon.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStartShootingSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStopShootingSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnShootingSignature, int&, AmmoValue);

UCLASS()
class MULTIPLAYERPROJECT_API ABaseWeapon : public AActor, public IInteractableInterface
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
	bool bIsFiring;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "References")
	FName WeaponSocket;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "References")
	class AActor* mOwnerRef;


	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Private")
	FWeaponDetails mWeaponDetails;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Private")
	FInteractableDetails mInteractableDetails;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, BlueprintAssignable, Category = "Private")
	FStartShootingSignature StartShootingSignature;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, BlueprintAssignable, Category = "Private")
	FStopShootingSignature StopShootingSignature;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, BlueprintAssignable, Category = "Private")
	FOnShootingSignature OnShootingSignature;

	// Weapon related Properties
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weapon Properties")
	TEnumAsByte<ETraceTypeQuery> TraceChannel;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weapon Properties")
	float TraceRange;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weapon Properties")
	float DamageRate;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weapon Properties")
	FTimerHandle TimeHandler;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weapon Properties")
	float mBulletSpeed;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weapon Properties")
	int Ammo;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weapon Properties")
	float mInFirstDelay;

public:
	// Engine methods
	
	virtual void BeginPlay() override;
	
public:
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Init();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnComponentBeginOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* Actor, UPrimitiveComponent* PrimitiveComponent1, int I, bool bArg, const FHitResult& HitResult);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnComponentEndOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* Actor, UPrimitiveComponent* PrimitiveComponent1, int I);

	virtual FInteractableDetails GetInteractableDetails_Implementation() override;
	virtual void Interact_Implementation(AActor* OwnerPlayer) override;

	// -------------------------------------------------------------------------------------

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void PlayWeaponSound();
	
	// Attaching the weapon
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void AttachWeaponToPlayer(AActor* OwnerPlayer);

	UFUNCTION(Server, Reliable)
	void Server_AttachWeaponToPlayer(AActor* OwnerPlayer);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_AttachWeaponToPlayer(AActor* OwnerPlayer);

	
	// Fire
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Fire();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void StopFire();
	
	// Server Fire
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_Fire();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BlueprintServer_Fire(FHitResult hit);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_StopFire();

	// Multicast Fire
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
	void Multicast_Fire();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Blueprint_Multicast_Fire();

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
	void Multicast_StopFire();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void Blueprint_Multicast_StopFire();
};