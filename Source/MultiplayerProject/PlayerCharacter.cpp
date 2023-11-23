// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "EnhancedInputComponent.h"
#include "InputController.h"
#include "BaseClasses/BaseWeapon.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "DataAssetClasses/DA_InputData.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InterfaceClasses/PlayerStateInterface.h"
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

	FlagSocket = "flagSocket";
	bReplicates = true;
	bIsDead = false;
	//mHealth = 100.f;
	//Damage.AddDynamic(this, &APlayerCharacter::OnDamageTaken);

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
	if(UKismetSystemLibrary::DoesImplementInterface(Actor, UInteractableInterface::StaticClass()))
	{
		CollidedActor = Actor;

		UE_LOG(LogTemp, Warning, TEXT("Collision Actor Name: %s"), *CollidedActor->GetName());
	}
}
void APlayerCharacter::OnOverlapEnd_Implementation(UPrimitiveComponent* PrimitiveComponent, AActor* Actor, UPrimitiveComponent* PrimitiveComponent1, int I)
{
	CollidedActor = nullptr;
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

void APlayerCharacter::DropItem_Implementation()
{
	
}

void APlayerCharacter::FlagSpawner_Implementation(AActor* FlagRef)
{

	
}

#pragma region When the player Interacts

void APlayerCharacter::Server_Interact_Implementation()
{
	Client_Interact();
}

void APlayerCharacter::Client_Interact_Implementation()
{
	if(CollidedActor == nullptr) return;
	
	IInteractableInterface::Execute_Interact(CollidedActor, this);
	CollidedActor = nullptr;
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
	if(mPrimaryWeapon == nullptr) return;
	bIsFiring = true;
	mPrimaryWeapon->StartShootingSignature.Broadcast();
}

void APlayerCharacter::Multicast_StopShoot_Implementation()
{
	if(mPrimaryWeapon == nullptr) return;
	bIsFiring = false;
	mPrimaryWeapon->StopShootingSignature.Broadcast();
}

#pragma endregion

#pragma region When the player Spawns weapon

void APlayerCharacter::Server_SpawnWeapon_Implementation(FWeaponDetails WeaponDetails)
{
	if(mPrimaryWeapon) mPrimaryWeapon->Destroy();
	
	FActorSpawnParameters SpawnsParams;
	SpawnsParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::Undefined;

	ABaseWeapon* WeaponToSpawn = Cast<ABaseWeapon>(GetWorld()->SpawnActor(WeaponDetails.WeaponAsset, &GetActorTransform(), SpawnsParams));
	WeaponToSpawn->SetOwner(this);
	WeaponToSpawn->SetInstigator(this);

	WeaponToSpawn->AttachWeaponToPlayer(this);

	BlueprintServer_SpawnWeapon(WeaponToSpawn);
	Execute_SetWeapon(this, WeaponToSpawn);
}

#pragma endregion

#pragma region Getters and Setters

UCameraComponent* APlayerCharacter::GetFollowCamera_Implementation()
{
	return FollowCamera;
}

UMeshComponent* APlayerCharacter::GetMeshComponent_Implementation()
{
	return GetMesh();
}

ABaseWeapon* APlayerCharacter::GetWeapon_Implementation()
{
	return mPrimaryWeapon;
}

void APlayerCharacter::SetWeapon_Implementation(ABaseWeapon* Weapon)
{
	mPrimaryWeapon = Weapon;
}

#pragma endregion 

bool APlayerCharacter::IsDead_Implementation()
{
	return bIsDead;
}

void APlayerCharacter::Dead_Implementation(AController* InstigatedBy)
{
	Multicast_OnDead(InstigatedBy);
}

void APlayerCharacter::Multicast_OnDead_Implementation(AController* InstigatedBy)
{
	BlueprintMulticast_OnDead(InstigatedBy);
}

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

void APlayerCharacter::RefreshPawn_Implementation()
{
	
}

