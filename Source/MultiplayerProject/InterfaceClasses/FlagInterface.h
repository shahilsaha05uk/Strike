// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MultiplayerProject/EnumClass.h"
#include "UObject/Interface.h"
#include "FlagInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UFlagInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MULTIPLAYERPROJECT_API IFlagInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	EFlagStatus GetFlagStatus();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetFlagStatus(EFlagStatus UpdatedStatus);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void CaptureFlag(USkeletalMeshComponent* MeshToAttachTo, FName SocketName);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void RetrieveFlag();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void MisplaceFlag();
};
