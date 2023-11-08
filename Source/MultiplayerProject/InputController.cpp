// Fill out your copyright notice in the Description page of Project Settings.


#include "InputController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "DataAssetClasses/DA_InputData.h"
#include "DataAssetClasses/DA_UIInputs.h"
#include "InterfaceClasses/PlayerInputInterface.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"

AInputController::AInputController()
{
	//bReplicates = true;
}

void AInputController::BeginPlay()
{
	//Init();
	Super::BeginPlay();
}
void AInputController::Init_Implementation()
{
	PlayerCameraManager->ViewPitchMin = mMinCamPitch;
	PlayerCameraManager->ViewPitchMax = mMaxCamPitch;
	
	mHudRef = Cast<AMP_HUD>(GetHUD());
}

void AInputController::SetupInputComponent()
{
	Super::SetupInputComponent();

	
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent)) {
		
		//Jumping
		EnhancedInputComponent->BindAction(InputData->IA_Jump, ETriggerEvent::Triggered, this, &AInputController::Jumping);
		EnhancedInputComponent->BindAction(InputData->IA_Jump, ETriggerEvent::Completed, this, &AInputController::StopJump);

		//Moving
		EnhancedInputComponent->BindAction(InputData->IA_Move, ETriggerEvent::Triggered, this, &AInputController::Move);

		//Looking
		EnhancedInputComponent->BindAction(InputData->IA_Look, ETriggerEvent::Triggered, this, &AInputController::Look);

		// Pickup
		EnhancedInputComponent->BindAction(InputData->IA_Pickup, ETriggerEvent::Completed, this, &AInputController::Pickup);

		// Aiming
		EnhancedInputComponent->BindAction(InputData->IA_Aim, ETriggerEvent::Started, this, &AInputController::StartAiming);
		EnhancedInputComponent->BindAction(InputData->IA_Aim, ETriggerEvent::Completed, this, &AInputController::StopAiming);

		// Shooting
		EnhancedInputComponent->BindAction(InputData->IA_Shoot, ETriggerEvent::Started, this, &AInputController::StartShooting);
		EnhancedInputComponent->BindAction(InputData->IA_Shoot, ETriggerEvent::Completed, this, &AInputController::StopShooting);


		// ======= UI Inputs =======
		EnhancedInputComponent->BindAction(UIInputs->IA_Pause, ETriggerEvent::Completed, this, &AInputController::PauseGame);
		EnhancedInputComponent->BindAction(UIInputs->IA_Shop, ETriggerEvent::Completed, this, &AInputController::OpenShop);

	}
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		// add the mapping context so we get controls
		Subsystem->AddMappingContext(InputMappingContext, 0);
		Subsystem->AddMappingContext(UI_MappingContext, 0);
		

		UE_LOG(LogTemp, Warning, TEXT("BeginPlay"));
	}


}

/*
void AInputController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	DOREPLIFETIME( AInputController, mHudRef );
}

*/
void AInputController::Server_Init_Implementation()
{
	Multicast_Init();
}

void AInputController::Multicast_Init_Implementation()
{
	Init();
}

#pragma region Player Action Methods

void AInputController::Move_Implementation(const FInputActionValue& Value)
{
	APawn* pawn = GetPawn();

	if(UKismetSystemLibrary::DoesImplementInterface(pawn, UPlayerInputInterface::StaticClass()))
	{
		IPlayerInputInterface::Execute_Move(pawn, Value);
	}
}

void AInputController::Look_Implementation(const FInputActionValue& Value)
{
	APawn* pawn = GetPawn();

	if(UKismetSystemLibrary::DoesImplementInterface(pawn, UPlayerInputInterface::StaticClass()))
	{
		IPlayerInputInterface::Execute_Look(pawn, Value);
	}
}

void AInputController::Jumping_Implementation(const FInputActionValue& Value)
{
	APawn* pawn = GetPawn();

	if(UKismetSystemLibrary::DoesImplementInterface(pawn, UPlayerInputInterface::StaticClass()))
	{
		IPlayerInputInterface::Execute_Jumping(pawn, Value);
	}
}

void AInputController::StopJump_Implementation()
{
	APawn* pawn = GetPawn();

	if(UKismetSystemLibrary::DoesImplementInterface(pawn, UPlayerInputInterface::StaticClass()))
	{
		IPlayerInputInterface::Execute_StopJump(pawn);
	}
}

void AInputController::Pickup_Implementation()
{
	APawn* pawn = GetPawn();

	if(UKismetSystemLibrary::DoesImplementInterface(pawn, UPlayerInputInterface::StaticClass()))
	{
		IPlayerInputInterface::Execute_Pickup(pawn);
	}

}

void AInputController::StartAiming_Implementation()
{
	APawn* pawn = GetPawn();

	if(UKismetSystemLibrary::DoesImplementInterface(pawn, UPlayerInputInterface::StaticClass()))
	{
		IPlayerInputInterface::Execute_StartAiming(pawn);
	}
}

void AInputController::StopAiming_Implementation()
{
	APawn* pawn = GetPawn();

	if(UKismetSystemLibrary::DoesImplementInterface(pawn, UPlayerInputInterface::StaticClass()))
	{
		IPlayerInputInterface::Execute_StopAiming(pawn);
	}
}

void AInputController::StartShooting_Implementation()
{
	APawn* pawn = GetPawn();

	if(UKismetSystemLibrary::DoesImplementInterface(pawn, UPlayerInputInterface::StaticClass()))
	{
		IPlayerInputInterface::Execute_StartShooting(pawn);
	}

}

void AInputController::StopShooting_Implementation()
{
	APawn* pawn = GetPawn();

	if(UKismetSystemLibrary::DoesImplementInterface(pawn, UPlayerInputInterface::StaticClass()))
	{
		IPlayerInputInterface::Execute_StopShooting(pawn);
	}
}

#pragma endregion

#pragma region UI Action Methods

void AInputController::PauseGame_Implementation()
{
	
}

void AInputController::TestAction_Implementation()
{
	
}

void AInputController::OpenShop_Implementation()
{
	
}

#pragma endregion
