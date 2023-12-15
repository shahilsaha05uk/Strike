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
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPawnDeadSignature, AController*, InstigatorController);

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
	APlayerCharacter* mPlayerRef;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "References")
	class AMP_PlayerState* mPlayerState;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "References")
	bool bHasRestarted;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "References")
	AHUD* mHudRef;

	UPROPERTY(BlueprintReadWrite, BlueprintCallable, BlueprintAssignable, EditAnywhere, Category = "References")
	FSpawnWeaponSignature SpawnWeaponSignature;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, BlueprintReadWrite, EditAnywhere, Category = "References")
	FOnPawnDeadSignature OnPawnDeadSignature;

	UPROPERTY(BlueprintReadWrite, Category = "References")
	UDA_CharacterMeshDetails* CharacterMeshDetails;;
	
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputMappingContext* InputMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputMappingContext* UI_MappingContext;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Input Properties")
	float mMinCamPitch;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Input Properties")
	float mMaxCamPitch;

	AInputController();

#pragma region On Controller Spawn Methods
	
	virtual void BeginPlay() override;
	virtual void PawnSetup_Implementation(UDA_CharacterMeshDetails* CharacterDetails = nullptr, bool Restarting = false) override;

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

#pragma endregion

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

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void AddAmmo(int Value);
#pragma endregion

#pragma region UI Actions
	// UI Actions
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void PauseGame();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OpenShop();


#pragma endregion


// Weapon Related Methods

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnShooting(int& AmmoValue);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnSpawnWeapon(FWeaponDetails WeaponDetails);


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
	void Client_PostPossessed(FPlayerDetails PlayerDetails, FMatchDetails MatchDetails);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BlueprintClient_PostPossessed(FPlayerDetails PlayerDetails, FMatchDetails MatchDetails);
	
	
// When the player Dies

	virtual void OnPlayerDead_Implementation(AController* InstigatorController) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void RequestNewPlayer();
	
	virtual void RestartPlayer_Implementation() override;

// Ask the Game Mode to take a game decision when the player Dies

	UFUNCTION(Server, Reliable)
	void Server_RequestGameModeDecision(AController* InstigatorController);
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BlueprintServer_RequestGameModeDecision(AController* InstigatorController);

// On session End

	virtual void OnSessionEnd_Implementation(ETeam WinningTeam, int TScore, int CTScore) override;
	
	virtual void UpdatePlayerHUDDetails_Implementation(EHUDValue ValueType, const FString& Value) override;

	UFUNCTION(Client, Reliable)
	void Client_UpdatePlayerHUDDetails(EHUDValue ValueType, const FString& Value);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BlueprintClient_UpdatePlayerHUDDetails(EHUDValue ValueType, const FString& Value);
};

