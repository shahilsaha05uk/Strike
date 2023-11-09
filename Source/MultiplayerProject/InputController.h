// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "MP_HUD.h"
#include "GameFramework/PlayerController.h"
#include "InterfaceClasses/ControllerInterface.h"
#include "InputController.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERPROJECT_API AInputController : public APlayerController, public IControllerInterface
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Input Properties")
	class UDA_InputData* InputData;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Input Properties")
	class UDA_UIInputs* UIInputs;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "References")
	APawn* mPlayerRef;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "References")
	AHUD* mHudRef;
	
	
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
	
	virtual void OnSpawn_Implementation() override;
	virtual void SpawnPawn_Implementation(ETeam Team) override;
	virtual void UpdatePlayerState_Implementation() override;
	
	virtual void SetupInputComponent() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Init();

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
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Pickup();
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

public:

	// Server methods
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_OnSpawn();

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_SpawnPawn(ETeam Team);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BlueprintServer_SpawnPawn(TSubclassOf<APawn> DefaultPawnClass, ETeam Team, const FTransform& FindStartTransform);
	
public:
	
	// Multicast Methods
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BlueprintMulticast_OnSpawn();

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
	void Multicast_SpawnPawn(TSubclassOf<APawn> DefaultPawnClass, ETeam Team, const FTransform& FindStartTransform);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BlueprintMulticast_SpawnPawn(TSubclassOf<APawn> DefaultPawnClass, ETeam Team, const FTransform& FindStartTransform);

public:
	
	// Client Methods
	UFUNCTION(Client, Reliable, BlueprintCallable)
	void Client_OnSpawn();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BlueprintClient_OnSpawn();

};

