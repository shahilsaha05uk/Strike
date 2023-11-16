// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "EnhancedInputComponent.h"
#include "BaseClasses/BaseWeapon.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "DataAssetClasses/DA_InputData.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InterfaceClasses/PickupInterface.h"
#include "InterfaceClasses/PlayerStateInterface.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"

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

	mOverlayWidget = CreateDefaultSubobject<UWidgetComponent>("Overlay UI");
	mOverlayWidget->SetupAttachment(RootComponent);
	
	WeaponSocket = "weapon_r";
	FlagSocket = "flagSocket";
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void APlayerCharacter::Init_Implementation()
{
}


void APlayerCharacter::OnOverlapBegin_Implementation(UPrimitiveComponent* PrimitiveComponent, AActor* Actor, UPrimitiveComponent* PrimitiveComponent1, int I, bool bArg, const FHitResult& HitResult)
{
	/*
	if(UKismetSystemLibrary::DoesImplementInterface(Actor, UPickupInterface::StaticClass()))
	{
		const EInteractType InteractType = IPickupInterface::Execute_GetInteractType(Actor);

		FocusedActorDetails = {};
		
		FocusedActorDetails.ActorName = Actor->GetName();
		FocusedActorDetails.InteractType = InteractType;
		FocusedActorDetails.ActorReference = Actor;
	}
*/
}
void APlayerCharacter::OnOverlapEnd_Implementation(UPrimitiveComponent* PrimitiveComponent, AActor* Actor, UPrimitiveComponent* PrimitiveComponent1, int I)
{
	/*if(UKismetSystemLibrary::DoesImplementInterface(Actor, UPickupInterface::StaticClass()))
	{
		FocusedActorDetails = {};
	}*/
}

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
}
void APlayerCharacter::StopAiming_Implementation()
{
	isAiming = false;
}

void APlayerCharacter::StartShooting_Implementation()
{
	Server_Shoot();
}

void APlayerCharacter::StopShooting_Implementation()
{
	Server_StopShoot();
}

void APlayerCharacter::SpawnWeapon_Implementation(FWeaponDetails WeaponDetails)
{
	Server_SpawnWeapon(WeaponDetails);
}

void APlayerCharacter::Interact_Implementation()
{
	Server_Interact();
}

UCameraComponent* APlayerCharacter::GetFollowCamera_Implementation()
{
	return FollowCamera;
}

UMeshComponent* APlayerCharacter::GetMeshComponent_Implementation()
{
	return GetMesh();
}

void APlayerCharacter::UpdateOverlayUI_Implementation()
{
	
}

void APlayerCharacter::InitHUD_Implementation(FPlayerDetails PlayerDetails)
{
	
}

void APlayerCharacter::UpdateFocusedActor_Implementation(FFocusedActorDetails Details)
{
	
}

ABaseWeapon* APlayerCharacter::GetWeapon_Implementation()
{
	return mPrimaryWeapon;
}

void APlayerCharacter::SetWeapon_Implementation(ABaseWeapon* Weapon)
{
	mPrimaryWeapon = Weapon;
}

#pragma region When the player Interacts

void APlayerCharacter::Server_Interact_Implementation()
{
	Client_Interact();
}

void APlayerCharacter::Client_Interact_Implementation()
{
	switch (FocusedActorDetails.InteractType)
	{
	case NOT_INTERACTABLE:

		break;
	case EQUIPPABLE:
		Equip();
		break;
	case COLLECTIBLE:
		Collect();
		break;
	}
	BlueprintClient_Interact();
}

#pragma endregion

#pragma region When the Player Shoots

void APlayerCharacter::Server_Shoot_Implementation()
{
	Multicast_Shoot();
}

void APlayerCharacter::Server_StopShoot_Implementation()
{
	Multicast_StopShoot();
}

void APlayerCharacter::Multicast_Shoot_Implementation()
{
	if(GetWeapon() == nullptr) return;
	bIsFiring = true;
	GetWeapon()->StartShootingSignature.Broadcast();
}

void APlayerCharacter::Multicast_StopShoot_Implementation()
{
	if(GetWeapon() == nullptr) return;
	bIsFiring = false;
	GetWeapon()->StopShootingSignature.Broadcast();
}

#pragma endregion

#pragma region When the player Spawns weapon

void APlayerCharacter::Server_SpawnWeapon_Implementation(FWeaponDetails WeaponDetails)
{
	Multicast_SpawnWeapon(WeaponDetails);
}

void APlayerCharacter::Multicast_SpawnWeapon_Implementation(FWeaponDetails WeaponDetails)
{
	BlueprintMulticast_SpawnWeapon(WeaponDetails);
}

#pragma endregion

#pragma region When the player Equips a weapon

void APlayerCharacter::Equip_Implementation()
{
	if(FocusedActorDetails.ActorReference == nullptr) return;

	if(GetWeapon() == nullptr && UKismetSystemLibrary::DoesImplementInterface(FocusedActorDetails.ActorReference, UPickupInterface::StaticClass()))
	{
		ABaseWeapon* weapon = Cast<ABaseWeapon>(FocusedActorDetails.ActorReference);

		SetWeapon(weapon);
	
		UMeshComponent* OwnerMesh = GetMesh();
	
		const FAttachmentTransformRules rules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, true);

		GetWeapon()->AttachToComponent(OwnerMesh, rules, WeaponSocket);

		if(UKismetSystemLibrary::DoesImplementInterface(GetWeapon(), UPickupInterface::StaticClass()))
		{
			IPickupInterface::Execute_OnInteract(GetWeapon());
		}
		//Server_PickupAndEquip(weapon);

		FocusedActorDetails = {};

	}
}

void APlayerCharacter::Server_PickupAndEquip_Implementation(ABaseWeapon* WeaponToEquip)
{
	Multicast_PickupAndEquip(WeaponToEquip);
}


void APlayerCharacter::Multicast_PickupAndEquip_Implementation(ABaseWeapon* WeaponToEquip)
{
	if(WeaponToEquip == nullptr) return;
	SetWeapon(WeaponToEquip);
	
	UMeshComponent* OwnerMesh = GetMesh();
	
	const FAttachmentTransformRules rules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, true);

	GetWeapon()->AttachToComponent(OwnerMesh, rules, WeaponSocket);

	if(UKismetSystemLibrary::DoesImplementInterface(GetWeapon(), UPickupInterface::StaticClass()))
	{
		IPickupInterface::Execute_OnInteract(GetWeapon());
	}
}

#pragma endregion

void APlayerCharacter::Collect_Implementation()
{
	
}



