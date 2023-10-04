// Fill out your copyright notice in the Description page of Project Settings.


#include "InputController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "DataAssetClasses/DA_InputData.h"
#include "InterfaceClasses/PlayerInputInterface.h"
#include "Kismet/KismetSystemLibrary.h"

void AInputController::BeginPlay()
{
	Init();
	Super::BeginPlay();
}
void AInputController::Init_Implementation()
{
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
	}
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		// add the mapping context so we get controls
		Subsystem->AddMappingContext(InputMappingContext, 0);

		UE_LOG(LogTemp, Warning, TEXT("BeginPlay"));
	}


}

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

