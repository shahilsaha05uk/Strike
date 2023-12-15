// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Character.h"
#include "InterfaceClasses/PlayerInputInterface.h"
#include "InterfaceClasses/PlayerInterface.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class MULTIPLAYERPROJECT_API APlayerCharacter : public ACharacter, public IPlayerInputInterface, public IPlayerInterface
{
	GENERATED_BODY()

private:

public:
	APlayerCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Pickup Property")
	class ABaseWeapon* mPrimaryWeapon;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup Property")
	class UInteractableComponent* mInteractableComp;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Properties")
	FTransform InitialCamTransform;

	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "References")
	class AInputController* mControllerRef;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "References")
	class AHUD* mHudRef;

	UPROPERTY(Replicated, BlueprintReadWrite, VisibleAnywhere)
	bool isAiming;
	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere)
	bool bIsDead;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	bool bIsFiring;
	virtual void RefreshPawn_Implementation() override;

	UFUNCTION(Server, Reliable)
	void Server_IsAiming(bool Value);
	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void BlueprintOnRep_IsAiming();

	//Engine Methods
	
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Init();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnOverlapEnd(UPrimitiveComponent* PrimitiveComponent, AActor* Actor, UPrimitiveComponent* PrimitiveComponent1, int I);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnOverlapBegin(UPrimitiveComponent* PrimitiveComponent, AActor* Actor, UPrimitiveComponent* PrimitiveComponent1, int I, bool bArg, const FHitResult& HitResult);
	
	// Player Inputs Override
	virtual void Move_Implementation(const FInputActionValue& Value) override;

	virtual void Look_Implementation(const FInputActionValue& Value) override;

	virtual void Jumping_Implementation(const FInputActionValue& Value) override;
	virtual void StopJump_Implementation() override;
	
	virtual void StartAiming_Implementation() override;
	virtual void StopAiming_Implementation() override;
	
	virtual void StartShooting_Implementation() override;
	virtual void OnShooting_Implementation(int val) override;
	virtual void StopShooting_Implementation() override;
	
	virtual void Interact_Implementation() override;
	virtual void DropItem_Implementation() override;

	// Player Interface
	virtual UCameraComponent* GetFollowCamera_Implementation() override;
	virtual UMeshComponent* GetMeshComponent_Implementation() override;

	
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
public:

	// Updating the Weapon Properties
	virtual void AddAmmo_Implementation(int Value) override;

	// Weapon Related methods
	virtual void SpawnWeapon_Implementation(FWeaponDetails WeaponDetails) override;
	virtual ABaseWeapon* GetWeapon_Implementation() override;
	virtual void SetWeapon_Implementation(ABaseWeapon* Weapon) override;

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_SpawnWeapon(FWeaponDetails WeaponDetails);
	
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
	void Multicast_SpawnWeapon(FWeaponDetails WeaponDetails, ABaseWeapon* Weapon);
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BlueprintServer_SpawnWeapon(ABaseWeapon* Weapon);
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BlueprintMulticast_SpawnWeapon(FWeaponDetails WeaponDetails, ABaseWeapon* Weapon);
	

	// When the player shoots
	UFUNCTION(Server, Unreliable, BlueprintCallable)
	void Server_Shoot();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BlueprintClient_OnShoot();
	
	UFUNCTION(Server, Unreliable, BlueprintCallable)
	void Server_StopShoot();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BlueprintClient_OnStopShoot();
	
	
	UFUNCTION(NetMulticast, Unreliable, BlueprintCallable)
	void Multicast_Shoot();

	UFUNCTION(NetMulticast, Unreliable, BlueprintCallable)
	void Multicast_StopShoot();
	
	// Updating Overhead UI
	virtual void UpdateHealthBar_Implementation(float Health) override;

	UFUNCTION(Server, Unreliable, BlueprintCallable)
	void Server_UpdateHealthBar(float Health);
	UFUNCTION(NetMulticast, Unreliable, BlueprintCallable)
	void Multicast_UpdateHealthBar(float Health);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void BlueprintMulticast_UpdateHealthBar(float Health);
	
	// When the player dies
	virtual bool IsDead_Implementation() override;
	virtual void Dead_Implementation(AController* InstigatedBy) override;
	
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
	void Multicast_OnDead(AController* InstigatedBy);

	UFUNCTION(Client, Reliable, BlueprintCallable)
	void Client_OnDead(AController* InstigatedBy);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BlueprintClient_OnDead(AController* InstigatedBy);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BlueprintMulticast_OnDead(AController* InstigatedBy);

	UFUNCTION(BlueprintCallable)
	void OnDeadTimerFinished(AController* InstigatedBy);

	// On Session End
	virtual void OnSessionEnd_Implementation(ETeam WinningTeam, int TScore, int CTScore) override;

	// New Changes

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	TMap<TEnumAsByte<EInteractableItem>, class ABaseInteractable*> CurrentInteractableItem;

	virtual ABaseInteractable* GetInventoryItem_Implementation(EInteractableItem Item) override;

	UFUNCTION(Server, Reliable)
	void Server_Interact(EInteractAction ActionType, EInteractableItem Item, AActor* TargetActor = nullptr);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void RemoveFromInventory(EInteractableItem InteractableItem);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void UpdateInventory(EInteractableItem Item, ABaseInteractable* InteractableActor);
	
	virtual void OnAttachActor_Implementation(EInteractableItem ItemType, ABaseInteractable* NewActor) override;
	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	AActor* CollidedActor;


};
