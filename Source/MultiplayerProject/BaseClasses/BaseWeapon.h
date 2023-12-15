// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseInteractable.h"
#include "GameFramework/Actor.h"
#include "MultiplayerProject/StructClass.h"
#include "MultiplayerProject/InterfaceClasses/WeaponInterface.h"
#include "NiagaraComponent.h"
#include "MultiplayerProject/DataAssetClasses/DA_WeaponDetails.h"
#include "BaseWeapon.generated.h"

UCLASS()
class MULTIPLAYERPROJECT_API ABaseWeapon : public ABaseInteractable, public IWeaponInterface
{
	GENERATED_BODY()

private:


public:
	ABaseWeapon();
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Actor Components")
	UNiagaraComponent* mParticleComponent;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Actor Components")
	class USphereComponent* mCollisionComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Actor Components")
	class UWidgetComponent* mUIComponent;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Actor Components")
	class USkeletalMeshComponent* mWeapon;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Actor Components")
	TSubclassOf<UUserWidget> mWidgetClass;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "References")
	FName mWeaponSocket;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "References")
	FName mMuzzleSocket;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "References")
	class AActor* mOwnerRef;


	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Private")
	FInteractableDetails mInteractableDetails;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Private")
	TEnumAsByte<ETraceTypeQuery> TraceChannel;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Private")
	FTimerHandle TimeHandler;

	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere)
	FWeaponDetails mWeaponDetails;
	
	
	// Weapon related Properties
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Private")
	class UDA_WeaponDetails* WeaponAsset;
	
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weapon Properties")
	float mInFirstDelay;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weapon Properties")
	float mSpread;
	
	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere, Category = "Weapon Properties")
	float mFireRate;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weapon Properties")
	float TraceRange;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weapon Properties")
	float MuzzleDuration;

	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere, Category = "Weapon Properties")
	USoundBase* mWeaponSound;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weapon Properties")
	float mDamageRate;
	
	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere, Category = "Weapon Properties")
	int mAmmo;

	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere, Category = "Weapon Properties")
	bool bIsPickedUp;


	
public:
	// Engine methods
	
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
public:
	
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Request_HUDUpdate();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Init();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnComponentBeginOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* Actor, UPrimitiveComponent* PrimitiveComponent1, int I, bool bArg, const FHitResult& HitResult);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnComponentEndOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* Actor, UPrimitiveComponent* PrimitiveComponent1, int I);

	virtual void Interact_Implementation(ACharacter* OwnerPlayer) override;
	virtual void DropItem_Implementation(ACharacter* OwnerPlayer) override;
	virtual void AttachToPlayer_Implementation(ACharacter* OwnerPlayer) override;
	virtual void DetachFromPlayer_Implementation(ACharacter* OwnerPlayer) override;
	
	// -------------------------------------------------------------------------------------

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	ABaseWeapon* SpawnForPlayer(ACharacter* OwnerPlayer, FWeaponDetails WeaponDetails);

	UFUNCTION(Client, Reliable)
	void Client_Interact(ACharacter* OwnerPlayer, ABaseWeapon* WeaponRef, bool UIVisibility);
	
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_Interact(ACharacter* OwnerPlayer, ABaseWeapon* WeaponRef, bool UIVisibility);
	
	
	UFUNCTION(Server, Reliable)
	void Server_AddAmmo(int Value);

	
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void WeaponSetup(ACharacter* OwnerPlayer, ABaseWeapon* Weapon);	// Should only be called from a server method

	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void PlayWeaponSound();
	
	// Fire

	virtual void Fire_Implementation() override;
	virtual void StopFire_Implementation() override;

	
	// Server Fire
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_Fire();

	UFUNCTION(Client, Reliable, BlueprintCallable)
	void Client_Fire();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BlueprintServer_Fire(FHitResult hit);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_StopFire();

	// Multicast Fire
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
	void Multicast_Fire(FHitResult hit);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void Blueprint_Multicast_Fire(FHitResult hit);

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
	void Multicast_StopFire();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void Blueprint_Multicast_StopFire();


	UFUNCTION(BlueprintPure, BlueprintCallable)
	FVector SpreadTrace(FVector InputTrace);


	// New Changes
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void UpdateWeaponProperties(FWeaponDetails WeaponDetails, ABaseWeapon* WeaponRef);
	UFUNCTION(Client, Reliable)
	void Client_UpdateWeaponProperties(FWeaponDetails WeaponDetails, ABaseWeapon* WeaponRef);
	
};