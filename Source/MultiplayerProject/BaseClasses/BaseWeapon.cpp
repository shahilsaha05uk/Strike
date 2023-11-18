// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWeapon.h"

#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MultiplayerProject/PlayerCharacter.h"
#include "MultiplayerProject/InterfaceClasses/PlayerInputInterface.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ABaseWeapon::ABaseWeapon()
{
	mCollisionComponent = CreateDefaultSubobject<USphereComponent>("CollisionComponent");
	RootComponent = mCollisionComponent;
	
	mProjectileSpawnLocation = CreateDefaultSubobject<USceneComponent>("ProjectileSpawn");
	mProjectileSpawnLocation->SetupAttachment(RootComponent);
	
	mCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ABaseWeapon::OnComponentBeginOverlap);
	mCollisionComponent->OnComponentEndOverlap.AddDynamic(this, &ABaseWeapon::OnComponentEndOverlap);

	mUIComponent = CreateDefaultSubobject<UWidgetComponent>("Popup Component");
	mUIComponent->SetupAttachment(mCollisionComponent);
	mUIComponent->SetDrawAtDesiredSize(true);
	mUIComponent->SetWidgetClass(mWidgetClass);
	mUIComponent->SetTickMode(ETickMode::Automatic);
	mUIComponent->SetVisibility(false);

	WeaponSocket = "weapon_r";

	StartShootingSignature.AddDynamic(this, &ThisClass::Fire);
	StopShootingSignature.AddDynamic(this, &ThisClass::StopFire);
	//UKismetSystemLibrary::Timer
}

void ABaseWeapon::BeginPlay()
{
	Init();
	
	Super::BeginPlay();
}


void ABaseWeapon::Init_Implementation()
{
	bIsFiring = false;
	mInteractableDetails.ActorName = GetName();
	mInteractableDetails.ActorReference = this;
	mInteractableDetails.InteractType = EQUIPPABLE;
}

void ABaseWeapon::OnComponentBeginOverlap_Implementation(UPrimitiveComponent* PrimitiveComponent, AActor* Actor, UPrimitiveComponent* PrimitiveComponent1, int I, bool bArg, const FHitResult& HitResult)
{
	if(UKismetSystemLibrary::DoesImplementInterface(Actor, UPlayerInterface::StaticClass()))
	{
		mUIComponent->SetVisibility(true);
	}
}

void ABaseWeapon::OnComponentEndOverlap_Implementation(UPrimitiveComponent* PrimitiveComponent, AActor* Actor,
	UPrimitiveComponent* PrimitiveComponent1, int I)
{
	if(UKismetSystemLibrary::DoesImplementInterface(Actor, UPlayerInterface::StaticClass()))
	{
		mUIComponent->SetVisibility(false);
	}
}

void ABaseWeapon::PlayWeaponSound_Implementation()
{
}

void ABaseWeapon::AttachWeaponToPlayer_Implementation(AActor* OwnerPlayer)
{
	mOwnerRef = OwnerPlayer;
	
	const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, true);

	UMeshComponent* OwnerMesh = IPlayerInterface::Execute_GetMeshComponent(OwnerPlayer);

	IPlayerInterface::Execute_SetWeapon(OwnerPlayer, this);

	AttachToComponent(OwnerMesh, AttachmentRules, WeaponSocket);
}

void ABaseWeapon::Multicast_AttachWeaponToPlayer_Implementation(AActor* OwnerPlayer)
{
}



#pragma region When Weapon Is Fired

// Fire
void ABaseWeapon::Fire_Implementation()
{
	Server_Fire();
}

void ABaseWeapon::StopFire_Implementation()
{
	Server_StopFire();
}

// Server Fire
void ABaseWeapon::Server_Fire_Implementation()
{
	Multicast_Fire();
}

void ABaseWeapon::Server_StopFire_Implementation()
{
	Multicast_StopFire();
}

// Multicast Fire
void ABaseWeapon::Multicast_Fire_Implementation()
{
	Blueprint_Multicast_Fire();
}

void ABaseWeapon::Multicast_StopFire_Implementation()
{
	Blueprint_Multicast_StopFire();
}

// BP Multicast
void ABaseWeapon::Blueprint_Multicast_Fire_Implementation()
{
}

void ABaseWeapon::Blueprint_Multicast_StopFire_Implementation()
{
}

#pragma endregion

#pragma region Interface Implementation

FInteractableDetails ABaseWeapon::GetInteractableDetails_Implementation()
{
	return 	mInteractableDetails;
}

void ABaseWeapon::Interact_Implementation(AActor* OwnerPlayer)
{
	if(UKismetSystemLibrary::DoesImplementInterface(OwnerPlayer, UPlayerInterface::StaticClass()))
	{
		AttachWeaponToPlayer(OwnerPlayer);
		
		mCollisionComponent->SetVisibility(false);
		mCollisionComponent->SetGenerateOverlapEvents(false);
	}
}

#pragma endregion

