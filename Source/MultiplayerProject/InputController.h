// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/PlayerController.h"
#include "InputController.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERPROJECT_API AInputController : public APlayerController
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Input Properties")
	class UDA_InputData* InputData;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Input Properties")
	class UDA_UIInputs* UIInputs;
	
	
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

	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Init();

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

	// UI Actions
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void PauseGame();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OpenShop();

	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void TestAction();

};
