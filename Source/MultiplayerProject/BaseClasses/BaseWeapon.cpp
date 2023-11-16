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
	
}

void ABaseWeapon::OnComponentBeginOverlap_Implementation(UPrimitiveComponent* PrimitiveComponent, AActor* Actor,
                                                         UPrimitiveComponent* PrimitiveComponent1, int I, bool bArg, const FHitResult& HitResult)
{
	if(UKismetSystemLibrary::DoesImplementInterface(Actor, UPlayerInputInterface::StaticClass()))
	{
		mOwnerRef = Actor;
	}
	
	mOwnerRef = Cast<APlayerCharacter>(Actor);
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

/*
ABaseWeapon* ABaseWeapon::EquipWeapon_Implementation()
{
	ServerEquip();

	/*
	if(mOwnerRef == nullptr) return nullptr;

	UMeshComponent* OwnerMesh = IPlayerInputInterface::Execute_GetMeshComponent(mOwnerRef);
	
	const FAttachmentTransformRules rules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, true);
	
	AttachToComponent(OwnerMesh, rules, WeaponSocket);

	mUIComponent->SetVisibility(false);
	
	return this;
}
*/
void ABaseWeapon::OnInteract_Implementation()
{
	
}

EInteractType ABaseWeapon::GetInteractType_Implementation()
{
	return EInteractType::EQUIPPABLE;
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
