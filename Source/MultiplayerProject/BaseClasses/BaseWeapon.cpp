// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWeapon.h"

#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MultiplayerProject/PlayerCharacter.h"
#include "MultiplayerProject/InterfaceClasses/PlayerInputInterface.h"

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
	if(mOwnerRef)
	{
		mUIComponent->SetVisibility(true);
	}
}

void ABaseWeapon::OnComponentEndOverlap_Implementation(UPrimitiveComponent* PrimitiveComponent, AActor* Actor,
	UPrimitiveComponent* PrimitiveComponent1, int I)
{
	if(UKismetSystemLibrary::DoesImplementInterface(Actor, UPlayerInputInterface::StaticClass()))
	{
		mUIComponent->SetVisibility(false);
	}
}


void ABaseWeapon::PlayWeaponSound_Implementation()
{
}

#pragma region Server Method Callers

void ABaseWeapon::Fire_Implementation()
{
}

void ABaseWeapon::StopFire_Implementation()
{
	
}
#pragma endregion

#pragma region Server Methods

void ABaseWeapon::Server_Fire_Implementation()
{
	Blueprint_Server_Fire();
}

void ABaseWeapon::Server_StopFire_Implementation()
{
	Blueprint_ServerStopFire();
}

void ABaseWeapon::Server_Equip_Implementation()
{
	Blueprint_Server_EquipWeapon();
}
#pragma endregion

#pragma region Multicast Methods

void ABaseWeapon::Multicast_Fire_Implementation()
{
	Blueprint_Multicast_Fire();
}

void ABaseWeapon::Multicast_StopFire_Implementation()
{
	Blueprint_Multicast_StopFire();
}

void ABaseWeapon::Multicast_EquipWeapon_Implementation()
{
	Blueprint_Multicast_EquipWeapon();
}
#pragma endregion

#pragma region Blueprint Server Method Implementation

void ABaseWeapon::Blueprint_Server_Fire_Implementation()
{
}

void ABaseWeapon::Blueprint_ServerStopFire_Implementation()
{
}

void ABaseWeapon::Blueprint_Server_EquipWeapon_Implementation()
{
	
}

#pragma endregion

#pragma region Blueprint Client Method Implementation

void ABaseWeapon::Blueprint_Multicast_Fire_Implementation()
{
}

void ABaseWeapon::Blueprint_Multicast_StopFire_Implementation()
{
}

void ABaseWeapon::Blueprint_Multicast_EquipWeapon_Implementation()
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
		const FAttachmentTransformRules rules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, true);

		UMeshComponent* OwnerMesh = IPlayerInterface::Execute_GetMeshComponent(OwnerPlayer);

		AttachToComponent(OwnerMesh, rules, WeaponSocket);

		IPlayerInterface::Execute_SetWeapon(OwnerPlayer, this);
	}
}

#pragma endregion

