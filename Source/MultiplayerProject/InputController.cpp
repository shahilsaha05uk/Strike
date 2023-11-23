// Fill out your copyright notice in the Description page of Project Settings.


#include "InputController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "MP_PlayerState.h"
#include "DataAssetClasses/DA_InputData.h"
#include "DataAssetClasses/DA_UIInputs.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/HUD.h"
#include "InterfaceClasses/PlayerInputInterface.h"
#include "InterfaceClasses/PlayerInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"
#include "WidgetClasses/PlayerHUD.h"


AInputController::AInputController()
{
	bReplicates = true;
}

void AInputController::BeginPlay()
{
	Super::BeginPlay();

	OnSpawn();
}

void AInputController::PawnSetup_Implementation(UDA_CharacterMeshDetails* CharacterDetails)
{
	Server_PawnSetup(CharacterDetails);
	SpawnPawn(CharacterDetails);
}

void AInputController::Server_PawnSetup_Implementation(UDA_CharacterMeshDetails* CharacterDetails)
{
	BlueprintServer_PawnSetup(CharacterDetails);
	
	Multicast_PawnSetup(CharacterDetails);
}

void AInputController::Multicast_PawnSetup_Implementation(UDA_CharacterMeshDetails* CharacterDetails)
{
	BlueprintMulticast_PawnSetup(CharacterDetails);
}

// Initialises the Controller when it spawns
// On Possess
void AInputController::OnPossess(APawn* InPawn)
{
	Client_PostPossessed();
	Super::OnPossess(InPawn);
}


// Updates the Player HUD
/*
void AInputController::UpdatePlayerHUD_Implementation(FPlayerDetails PlayerDetails)
{
	if(!mPlayerState) return;

	APawn* pawn = GetPawn();

	if(UKismetSystemLibrary::DoesImplementInterface(pawn, UInputsInterface::StaticClass()))
	{
		IInputsInterface::Execute_InitHUD(pawn, PlayerDetails);
	}
}
*/

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

		// Interact
		EnhancedInputComponent->BindAction(InputData->IA_Interact, ETriggerEvent::Completed, this, &AInputController::Interact);
		
		// Aiming
		EnhancedInputComponent->BindAction(InputData->IA_Aim, ETriggerEvent::Started, this, &AInputController::StartAiming);
		EnhancedInputComponent->BindAction(InputData->IA_Aim, ETriggerEvent::Completed, this, &AInputController::StopAiming);

		// Shooting
		EnhancedInputComponent->BindAction(InputData->IA_Shoot, ETriggerEvent::Started, this, &AInputController::StartShooting);
		EnhancedInputComponent->BindAction(InputData->IA_Shoot, ETriggerEvent::Completed, this, &AInputController::StopShooting);

		// Drop Item
		EnhancedInputComponent->BindAction(InputData->IA_DropItem, ETriggerEvent::Completed, this, &ThisClass::DropItem);

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

void AInputController::SetPlayerTeam_Implementation(ETeam Team)
{
	mPlayerTeam = Team;
}

void AInputController::RestartPlayer_Implementation()
{
	
}

void AInputController::OnShooting_Implementation(int& AmmoValue)
{
	
}

void AInputController::ShowScoreboard_Implementation()
{
	
}

void AInputController::UpdateScoreboard_Implementation(int Value, ETeam Team)
{
	
}

void AInputController::UpdatePlayerHUD_Implementation(FPlayerDetails PlayerDetails)
{
	AHUD* hud = GetHUD();
	if(UKismetSystemLibrary::DoesImplementInterface(hud, UHUDInterface::StaticClass()))
	{
		UPlayerHUD* PlayerHUD = Cast<UPlayerHUD>(IHUDInterface::Execute_GetWidget(hud, EWidgetType::PLAYER_HUD));
		PlayerHUD->UpdateMoney(PlayerDetails.CurrentMoney);
	}
}

void AInputController::UpdatePlayerHealthUI_Implementation(float Health)
{
}


void AInputController::DropItem_Implementation()
{
	APawn* pawn = GetPawn();

	if(UKismetSystemLibrary::DoesImplementInterface(pawn, UPlayerInputInterface::StaticClass()))
	{
		IPlayerInputInterface::Execute_DropItem(pawn);
	}

}

void AInputController::Interact_Implementation()
{
	APawn* pawn = GetPawn();

	if(UKismetSystemLibrary::DoesImplementInterface(pawn, UPlayerInputInterface::StaticClass()))
	{
		IPlayerInputInterface::Execute_Interact(pawn);
	}
}


void AInputController::OnSpawnWeapon_Implementation(FWeaponDetails WeaponDetails)
{
	APawn* pawn = GetPawn();

	if(UKismetSystemLibrary::DoesImplementInterface(pawn, UPlayerInterface::StaticClass()))
	{
		IPlayerInterface::Execute_SpawnWeapon(pawn, WeaponDetails);
	}

	mPlayerState->mPlayerDetails.CurrentMoney -= WeaponDetails.WeaponCost;
	Execute_UpdatePlayerHUD(this, mPlayerState->mPlayerDetails);
}

// Input Action Methods
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
	AHUD* hud = GetHUD();

	if(UKismetSystemLibrary::DoesImplementInterface(hud, UHUDInterface::StaticClass()))
	{
		UBaseWidget* PauseWidget = IHUDInterface::Execute_WidgetInitialiser(hud, EWidgetType::PAUSE_MENU);
		PauseWidget->AddToViewport();
	}
}

void AInputController::OpenShop_Implementation()
{
	AHUD* hud = GetHUD();

	if(UKismetSystemLibrary::DoesImplementInterface(hud, UHUDInterface::StaticClass()))
	{
		UBaseWidget* ShopWidget = IHUDInterface::Execute_WidgetInitialiser(hud, EWidgetType::SHOP_MENU);
		ShopWidget->ControllerRef = this;
		SpawnWeaponSignature.AddDynamic(this, &ThisClass::OnSpawnWeapon);
		ShopWidget->AddToViewport();
	}
}

#pragma endregion


// Controller Management Methods

#pragma region On Controller Spawn

void AInputController::OnSpawn()
{
	Server_OnSpawn();
}

void AInputController::Server_OnSpawn_Implementation()
{
	Client_OnSpawn();
}

void AInputController::Client_OnSpawn_Implementation()
{
	BlueprintClient_OnSpawn();
}

#pragma endregion

#pragma region Spawning the Player

// Spawns the Pawn and possess it
void AInputController::SpawnPawn(UDA_CharacterMeshDetails* CharacterDetails)
{
	Server_SpawnPawn(CharacterDetails);
}

void AInputController::Server_SpawnPawn_Implementation(UDA_CharacterMeshDetails* CharacterDetails)
{
	AGameModeBase* gameMode = UGameplayStatics::GetGameMode(GetWorld());
	const TSubclassOf<APawn> pawnClass = gameMode->DefaultPawnClass;
	const FTransform playerStart = gameMode->FindPlayerStart(this)->GetActorTransform();

	BlueprintServer_SpawnPawn(CharacterDetails, playerStart);
}

void AInputController::Client_PostPossessed_Implementation()
{
	mPlayerState = Cast<AMP_PlayerState>(PlayerState);

	PlayerCameraManager->ViewPitchMin = mMinCamPitch;
	PlayerCameraManager->ViewPitchMax = mMaxCamPitch;

	BlueprintClient_PostPossessed();
}

#pragma endregion
