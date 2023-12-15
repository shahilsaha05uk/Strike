// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "EnhancedInputComponent.h"
#include "InputController.h"
#include "ActorComponentClasses/InteractableComponent.h"
#include "BaseClasses/BaseInteractable.h"
#include "BaseClasses/BaseWeapon.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "DataAssetClasses/DA_InputData.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/SpringArmComponent.h"
#include "InterfaceClasses/FlagInterface.h"
#include "InterfaceClasses/PlayerStateInterface.h"
#include "InterfaceClasses/WeaponInterface.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"
#include "WidgetClasses/PlayerHUD.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnOverlapBegin);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &APlayerCharacter::OnOverlapEnd);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	bUseControllerRotationYaw = true;
	
	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	mInteractableComp = CreateDefaultSubobject<UInteractableComponent>("InteractableComp");
	bReplicates = true;
	bIsDead = false;
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	InitialCamTransform = FollowCamera->GetRelativeTransform();
}

void APlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APlayerCharacter, mPrimaryWeapon);
	DOREPLIFETIME(APlayerCharacter, bIsDead);
	DOREPLIFETIME(APlayerCharacter, isAiming);
}

void APlayerCharacter::Init_Implementation()
{
}

void APlayerCharacter::OnOverlapBegin_Implementation(UPrimitiveComponent* PrimitiveComponent, AActor* Actor, UPrimitiveComponent* PrimitiveComponent1, int I, bool bArg, const FHitResult& HitResult)
{
	if(UKismetSystemLibrary::DoesImplementInterface(Actor, UInteractableInterface::StaticClass()))
	{
		CollidedActor = Actor;
	}
}

void APlayerCharacter::OnOverlapEnd_Implementation(UPrimitiveComponent* PrimitiveComponent, AActor* Actor, UPrimitiveComponent* PrimitiveComponent1, int I)
{
	CollidedActor = nullptr;
}

#pragma region Player Inputs

void APlayerCharacter::Move_Implementation(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);

	}
}

void APlayerCharacter::Look_Implementation(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void APlayerCharacter::Jumping_Implementation(const FInputActionValue& Value)
{
	Jump();
}

void APlayerCharacter::StopJump_Implementation()
{
	StopJumping();
}

void APlayerCharacter::StartAiming_Implementation()
{
	isAiming = true;

	BlueprintOnRep_IsAiming();
}

void APlayerCharacter::StopAiming_Implementation()
{
	isAiming = false;

	BlueprintOnRep_IsAiming();
}

#pragma endregion

#pragma region When the Player Shoots

void APlayerCharacter::StartShooting_Implementation()
{
	Server_Shoot();
}

void APlayerCharacter::OnShooting_Implementation(int val)
{
	AController* C = GetController();

	if(UKismetSystemLibrary::DoesImplementInterface(C, UControllerInterface::StaticClass()))
	{
		IControllerInterface::Execute_UpdatePlayerHUDDetails(C, AMMO_Val, FString::FromInt(val));
	}
}

void APlayerCharacter::StopShooting_Implementation()
{
	Server_StopShoot();

	BlueprintClient_OnStopShoot();
}

void APlayerCharacter::Server_Shoot_Implementation()
{
	if(mPrimaryWeapon == nullptr) return;

	if(UKismetSystemLibrary::DoesImplementInterface(mPrimaryWeapon, UWeaponInterface::StaticClass()))
	{
		IWeaponInterface::Execute_Fire(mPrimaryWeapon);
		bIsFiring = true;
	}
	
}

void APlayerCharacter::Server_StopShoot_Implementation()
{
	Multicast_StopShoot();
}


void APlayerCharacter::Multicast_StopShoot_Implementation()
{
	if(mPrimaryWeapon == nullptr) return;
	if(UKismetSystemLibrary::DoesImplementInterface(mPrimaryWeapon, UWeaponInterface::StaticClass()))
	{
		IWeaponInterface::Execute_StopFire(mPrimaryWeapon);
		bIsFiring = false;
	}
}

#pragma endregion

// Updating the Weapon Properties
void APlayerCharacter::AddAmmo_Implementation(int Value)
{
	if(mPrimaryWeapon == nullptr) return;

	mPrimaryWeapon->Server_AddAmmo(Value);
}

// When the Player Dies
bool APlayerCharacter::IsDead_Implementation()
{
	return bIsDead;
}

void APlayerCharacter::Dead_Implementation(AController* InstigatedBy)
{
	Client_OnDead(InstigatedBy);
	
	Multicast_OnDead(InstigatedBy);
}

void APlayerCharacter::Client_OnDead_Implementation(AController* InstigatedBy)
{
	Server_Interact(DROP, FLAG);

	AController* C = GetController();
	
	if(UKismetSystemLibrary::DoesImplementInterface(C, UControllerInterface::StaticClass()))
	{
		IControllerInterface::Execute_UpdatePlayerHUDDetails(C, AMMO_Val, FString::FromInt(0));
	}
	BlueprintClient_OnDead(InstigatedBy);
}

void APlayerCharacter::Multicast_OnDead_Implementation(AController* InstigatedBy)
{

	BlueprintMulticast_OnDead(InstigatedBy);
	
	if(InstigatedBy == nullptr) return;
	FPlayerDetails PlayerDetails = IPlayerStateInterface::Execute_GetPlayerDetails(InstigatedBy->PlayerState);

	if(mPrimaryWeapon != nullptr)
	{
		mPrimaryWeapon->Destroy();
	}
	
	bIsDead = true;

	AController* CachedInstigatedBy = InstigatedBy;
	FTimerHandle TimerHandle;

	TWeakObjectPtr<APlayerCharacter> WeakThis(this);

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [WeakThis, CachedInstigatedBy]()
	{
		if (WeakThis.IsValid())
		{
			WeakThis->OnDeadTimerFinished(CachedInstigatedBy);
		}
	}, 5.0f, false);
}

void APlayerCharacter::OnDeadTimerFinished(AController* InstigatedBy)
{
	if(UKismetSystemLibrary::DoesImplementInterface(GetController(), UControllerInterface::StaticClass()))
	{
		IControllerInterface::Execute_OnPlayerDead(GetController(), InstigatedBy);
	}
}

void APlayerCharacter::RefreshPawn_Implementation()
{
	
}

// When the Player Is Aiming
void APlayerCharacter::Server_IsAiming_Implementation(bool Value)
{
}

// Updating the health Bar
void APlayerCharacter::UpdateHealthBar_Implementation(float Health)
{
	Server_UpdateHealthBar(Health);
}

void APlayerCharacter::Server_UpdateHealthBar_Implementation(float Health)
{
	Multicast_UpdateHealthBar(Health);
}

void APlayerCharacter::Multicast_UpdateHealthBar_Implementation(float Health)
{
	BlueprintMulticast_UpdateHealthBar(Health);
}

void APlayerCharacter::BlueprintMulticast_UpdateHealthBar_Implementation(float Health)
{
	
}

// On Session Ends Methods
void APlayerCharacter::OnSessionEnd_Implementation(ETeam WinningTeam, int TScore, int CTScore)
{
	AController* PC = GetController();
	if(UKismetSystemLibrary::DoesImplementInterface(PC, UControllerInterface::StaticClass()))
	{
		IControllerInterface::Execute_OnSessionEnd(PC, WinningTeam, TScore, CTScore);
	}
}

// New changes
void APlayerCharacter::Interact_Implementation()
{
	if(CollidedActor == nullptr) return;

	if(UKismetSystemLibrary::DoesImplementInterface(CollidedActor, UInteractableInterface::StaticClass()))
	{
		const EInteractableItem ItemType = IInteractableInterface::Execute_GetInteractableItem(CollidedActor);
		Server_Interact(INTERACT, ItemType, CollidedActor);
	}

	CollidedActor = nullptr;
}

void APlayerCharacter::DropItem_Implementation()
{
	Server_Interact(DROP, FLAG);
}

void APlayerCharacter::UpdateInventory_Implementation(EInteractableItem Item, ABaseInteractable* InteractableActor)
{
	if(CurrentInteractableItem.Contains(Item))
	{
		CurrentInteractableItem[Item] = InteractableActor;
		return;
	}

	CurrentInteractableItem.Add(Item, InteractableActor);
}

ABaseInteractable* APlayerCharacter::GetInventoryItem_Implementation(EInteractableItem Item)
{
	if(CurrentInteractableItem.Contains(Item)) return CurrentInteractableItem[Item];

	return nullptr;
}

void APlayerCharacter::Server_Interact_Implementation(EInteractAction ActionType, EInteractableItem Item, AActor* TargetActor)
{
	
	switch (ActionType) {
	case DROP:
		{
			ABaseInteractable* InteractableActor = Execute_GetInventoryItem(this, Item);
			if(InteractableActor)
			{
				mInteractableComp->DropItem(InteractableActor, this);
				RemoveFromInventory(InteractableActor->InteractableItem);
			}
		}
		break;
	case INTERACT:
		if(TargetActor)
		{
			mInteractableComp->Interact(TargetActor, this);
		}
		break;
	}
}

void APlayerCharacter::RemoveFromInventory_Implementation(EInteractableItem InteractableItem)
{
	if(CurrentInteractableItem.Contains(InteractableItem))
	{
		CurrentInteractableItem.Remove(InteractableItem);
	}
}

void APlayerCharacter::OnAttachActor_Implementation(EInteractableItem ItemType, ABaseInteractable* NewActor)
{
	UpdateInventory(ItemType, NewActor);
}

void APlayerCharacter::OnWeaponSpawn_Implementation()
{
	ABaseInteractable* Weapon = Execute_GetInventoryItem(this, WEAPON);

	mPrimaryWeapon = Cast<ABaseWeapon>(Weapon);
	
}

void APlayerCharacter::SpawnWeapon_Implementation(FWeaponDetails WeaponDetails)
{
	Server_SpawnWeapon(WeaponDetails);
}

void APlayerCharacter::Server_SpawnWeapon_Implementation(FWeaponDetails WeaponDetails)
{
	FActorSpawnParameters spawnParams;
	ABaseWeapon* Weapon = GetWorld()->SpawnActor<ABaseWeapon>(WeaponDetails.WeaponAsset, FVector::ZeroVector, FRotator::ZeroRotator, spawnParams);
	Weapon->WeaponSetup(this, Weapon);
}

#pragma region Getters and Setters

UCameraComponent* APlayerCharacter::GetFollowCamera_Implementation()
{
	return FollowCamera;
}

UMeshComponent* APlayerCharacter::GetMeshComponent_Implementation()
{
	return GetMesh();
}

#pragma endregion 

