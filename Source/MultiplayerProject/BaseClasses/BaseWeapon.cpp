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

	StartShootingSignature.AddDynamic(this, &ThisClass::ServerFire);
	StopShootingSignature.AddDynamic(this, &ThisClass::StopFire);
	//UKismetSystemLibrary::Timer
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

ABaseWeapon* ABaseWeapon::EquipWeapon_Implementation()
{
	if(mOwnerRef == nullptr) return nullptr;

	UMeshComponent* OwnerMesh = IPlayerInputInterface::Execute_GetMeshComponent(mOwnerRef);
	
	const FAttachmentTransformRules rules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, true);
	
	AttachToComponent(OwnerMesh, rules, WeaponSocket);

	mUIComponent->SetVisibility(false);
	
	return this;
}



void ABaseWeapon::PlayWeaponSound_Implementation()
{
}

void ABaseWeapon::StopFire_Implementation()
{
	
}
void ABaseWeapon::ServerFire_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Native MultiClient"));

	NativeServer_Fire();
}

void ABaseWeapon::MultiClientFire_Implementation()
{
}

void ABaseWeapon::NativeServer_Fire_Implementation()
{
	NativeMultiClient_Fire();
}

void ABaseWeapon::NativeMultiClient_Fire_Implementation()
{
	MultiClientFire();

}