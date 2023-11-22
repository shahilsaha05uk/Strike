// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "MP_HUD.h"
#include "StructClass.h"
#include "GameFramework/PlayerController.h"
#include "InterfaceClasses/ControllerInterface.h"
#include "InputController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSpawnWeaponSignature, FWeaponDetails, WeaponDetails);

UCLASS()
class MULTIPLAYERPROJECT_API AInputController : public APlayerController, public IControllerInterface
{
	GENERATED_BODY()

private:

	UFUNCTION()
	void OnSpawn();

	UFUNCTION()
	void SpawnPawn(UDA_CharacterMeshDetails* CharacterDetails);

	UPROPERTY()
	TEnumAsByte<ETeam> mPlayerTeam;


public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Input Properties")
	class UDA_InputData* InputData;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Input Properties")
	class UDA_UIInputs* UIInputs;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "References")
	APawn* mPlayerRef;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "References")
	class AMP_PlayerState* mPlayerState;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "References")
	AHUD* mHudRef;
	UPROPERTY(BlueprintReadWrite, BlueprintCallable, BlueprintAssignable, Category = "References")
	FSpawnWeaponSignature SpawnWeaponSignature;
	
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputMappingContext* InputMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputMappingContext* UI_MappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	bool shouldActivateMappingContext;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Input Properties")
	float mMinCamPitch;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Input Properties")
	float mMaxCamPitch;

	AInputController();

#pragma region On Controller Spawn Methods
	
	virtual void BeginPlay() override;
	virtual void PawnSetup_Implementation(UDA_CharacterMeshDetails* CharacterDetails = nullptr) override;

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Server_PawnSetup(UDA_CharacterMeshDetails* CharacterDetails);
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void Multicast_PawnSetup(UDA_CharacterMeshDetails* CharacterDetails);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BlueprintServer_PawnSetup(UDA_CharacterMeshDetails* CharacterDetails);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BlueprintMulticast_PawnSetup(UDA_CharacterMeshDetails* CharacterDetails);
	
	virtual void OnPossess(APawn* InPawn) override;

	virtual void SetupInputComponent() override;
	
	virtual void SetPlayerTeam_Implementation(ETeam Team) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnSpawnWeapon(FWeaponDetails WeaponDetails);

	virtual void RestartPlayer_Implementation() override;
#pragma endregion

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnShooting(int& AmmoValue);
	

	
	
#pragma region Player Actions

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Move(const FInputActionValue& Value);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Look(const FInputActionValue& Value);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Jumping(const FInputActionValue& Value);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void StopJump();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Interact();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void DropItem();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void StartAiming();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void StopAiming();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void StartShooting();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void StopShooting();

#pragma endregion

#pragma region UI Actions
	// UI Actions
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void PauseGame();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OpenShop();


#pragma endregion

// When the Controller Spawns

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_OnSpawn();

	UFUNCTION(Client, Reliable, BlueprintCallable)
	void Client_OnSpawn();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BlueprintClient_OnSpawn();

// When the Player Spawns
	
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_SpawnPawn(UDA_CharacterMeshDetails* CharacterDetails);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BlueprintServer_SpawnPawn(UDA_CharacterMeshDetails* CharacterDetails, const FTransform& FindStartTransform);

	// After the player is possessed
	UFUNCTION(Client, Reliable, BlueprintCallable)
	void Client_PostPossessed();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BlueprintClient_PostPossessed();


	// Updating the Player Scoreboard
	virtual void UpdateScoreboard_Implementation(int Value, ETeam Team) override;
	virtual void UpdatePlayerHUD_Implementation(FPlayerDetails PlayerDetails);
};

