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

	UPROPERTY(BlueprintReadWrite, BlueprintCallable, BlueprintAssignable, Category = "References")
	FSpawnWeaponSignature SpawnWeaponSignature;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, BlueprintReadWrite, Category = "References")
	FOnPawnDeadSignature OnPawnDeadSignature;

	UPROPERTY(BlueprintReadWrite, Category = "References")
	UDA_CharacterMeshDetails* CharacterMeshDetails;;
	
	
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
	void Client_PostPossessed(FPlayerDetails PlayerDetails);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BlueprintClient_PostPossessed(FPlayerDetails PlayerDetails);

	virtual void ShowScoreboard_Implementation(FPlayerDetails PlayerDetails) override;

// Updating the Player Scoreboard
	virtual void UpdateScoreboard_Implementation(int Value, ETeam Team) override;
	virtual void UpdatePlayerHUD_Implementation(FPlayerDetails PlayerDetails);
	virtual void UpdatePlayerHealthUI_Implementation(float Health);


// When the player Dies


	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnPlayerDead(AController* InstigatorController);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void RequestNewPlayer();
	
	virtual void RestartPlayer_Implementation() override;

// Ask the Game Mode to take a game decision when the player Dies

	UFUNCTION(Server, Reliable)
	void Server_RequestGameModeDecision(AController* InstigatorController);
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BlueprintServer_RequestGameModeDecision(AController* InstigatorController);
};

