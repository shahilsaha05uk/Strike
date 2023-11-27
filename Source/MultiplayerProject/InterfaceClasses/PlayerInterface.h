// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MultiplayerProject/StructClass.h"
#include "UObject/Interface.h"
#include "PlayerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPlayerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MULTIPLAYERPROJECT_API IPlayerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	// Only for debugging
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void FlagSpawner(AActor* FlagRef);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	UCameraComponent* GetFollowCamera();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	UMeshComponent* GetMeshComponent();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SpawnWeapon(FWeaponDetails WeaponDetails);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void RefreshPawn();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	ABaseWeapon* GetWeapon();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetWeapon(class ABaseWeapon* Weapon);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void AddAmmo(int Value);

	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void UpdateHealthBar(float Health);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Dead(AController* InstigatedBy);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool IsDead();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnSessionEnd(ETeam WinningTeam, int TScore, int CTScore);


};
