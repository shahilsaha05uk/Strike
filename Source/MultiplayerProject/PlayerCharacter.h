// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "InterfaceClasses/PlayerInputInterface.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class MULTIPLAYERPROJECT_API APlayerCharacter : public ACharacter, public IPlayerInputInterface
{
	GENERATED_BODY()

private:


public:
	APlayerCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Pickup Property")
	class ABaseWeapon* mFocusedPickupActor;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Pickup Property")
	class ABaseWeapon* mPrimaryWeapon;

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnOverlapEnd(UPrimitiveComponent* PrimitiveComponent, AActor* Actor, UPrimitiveComponent* PrimitiveComponent1, int I);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnOverlapBegin(UPrimitiveComponent* PrimitiveComponent, AActor* Actor, UPrimitiveComponent* PrimitiveComponent1, int I, bool bArg, const FHitResult& HitResult);

	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	ABaseWeapon* GetWeapon();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetWeapon(class ABaseWeapon* Weapon);
	
	virtual void Move_Implementation(const FInputActionValue& Value) override;
	virtual void Look_Implementation(const FInputActionValue& Value) override;
	virtual void Jumping_Implementation(const FInputActionValue& Value) override;
	virtual void StopJump_Implementation() override;
	virtual void Pickup_Implementation() override;

	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	
};
