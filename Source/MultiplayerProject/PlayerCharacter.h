// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "InterfaceClasses/InputsInterface.h"
#include "InterfaceClasses/PlayerInputInterface.h"
#include "InterfaceClasses/PlayerInterface.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class MULTIPLAYERPROJECT_API APlayerCharacter : public ACharacter, public IPlayerInputInterface, public IInputsInterface, public IPlayerInterface
{
	GENERATED_BODY()

private:

public:
	APlayerCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(ReplicatedUsing = OnRep_SetOwningRef, VisibleAnywhere, BlueprintReadWrite, Category = "Pickup Property")
	class ABaseWeapon* mPrimaryWeapon;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Pickup Property")
	class UWidgetComponent* mOverlayWidget;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "References")
	FName FlagSocket;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	bool isAiming;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	bool bIsFiring;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	AActor* CollidedActor;

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnRep_SetOwningRef();
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
	virtual void StopShooting_Implementation() override;
	virtual void SpawnWeapon_Implementation(FWeaponDetails WeaponDetails) override;
	virtual void Interact_Implementation() override;
	virtual void DropItem_Implementation() override;

	// Player Interface
	virtual UCameraComponent* GetFollowCamera_Implementation() override;
	virtual UMeshComponent* GetMeshComponent_Implementation() override;
	virtual ABaseWeapon* GetWeapon_Implementation() override;
	virtual void SetWeapon_Implementation(ABaseWeapon* Weapon) override;

	
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	
	virtual void UpdateOverlayUI_Implementation() override;
	virtual void InitHUD_Implementation(FPlayerDetails PlayerDetails) override;
	virtual void UpdateFocusedActor_Implementation(FInteractableDetails Details) override;
	virtual void FlagSpawner_Implementation(AActor* FlagRef) override;

public:
	
	// When the player shoots
	UFUNCTION(Server, Unreliable, BlueprintCallable)
	void Server_Shoot();

	UFUNCTION(Server, Unreliable, BlueprintCallable)
	void Server_StopShoot();
	
	UFUNCTION(NetMulticast, Unreliable, BlueprintCallable)
	void Multicast_Shoot();

	UFUNCTION(NetMulticast, Unreliable, BlueprintCallable)
	void Multicast_StopShoot();
	
	// When the player buys a weapon
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_SpawnWeapon(FWeaponDetails WeaponDetails);
	
	UFUNCTION(NetMulticast, Unreliable, BlueprintCallable)
	void Multicast_SpawnWeapon(FWeaponDetails WeaponDetails);

	// Interaction
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_Interact();
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
	void Client_Interact();
};
