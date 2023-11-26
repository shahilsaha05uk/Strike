// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWeapon.h"

#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MultiplayerProject/PlayerCharacter.h"
#include "MultiplayerProject/DataAssetClasses/DA_WeaponDetails.h"
#include "MultiplayerProject/InterfaceClasses/PlayerInputInterface.h"
#include "Net/UnrealNetwork.h"

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
	
	bReplicates = true;

	TraceRange = 20000.0f;
	
	WeaponSocket = "weapon_r";
}

void ABaseWeapon::BeginPlay()
{
	Init();
	
	Super::BeginPlay();
}

void ABaseWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABaseWeapon, mWeaponDetails);
	DOREPLIFETIME(ABaseWeapon, mAmmo);
	
}

void ABaseWeapon::Init_Implementation()
{
	bIsFiring = false;
	
	mInteractableDetails.ActorName = GetName();
	mInteractableDetails.ActorReference = this;
	mInteractableDetails.InteractType = EQUIPPABLE;

	//DamageRate = mWeaponDetails.WeaponDamage;
}

void ABaseWeapon::OnComponentBeginOverlap_Implementation(UPrimitiveComponent* PrimitiveComponent, AActor* Actor, UPrimitiveComponent* PrimitiveComponent1, int I, bool bArg, const FHitResult& HitResult)
{
	if(UKismetSystemLibrary::DoesImplementInterface(Actor, UPlayerInterface::StaticClass()))
	{
		mUIComponent->SetVisibility(true);
	}
}

void ABaseWeapon::OnComponentEndOverlap_Implementation(UPrimitiveComponent* PrimitiveComponent, AActor* Actor, UPrimitiveComponent* PrimitiveComponent1, int I)
{
	if(UKismetSystemLibrary::DoesImplementInterface(Actor, UPlayerInterface::StaticClass()))
	{
		if(mUIComponent)
		{
			mUIComponent->SetVisibility(false);
		}
	}
}

FInteractableDetails ABaseWeapon::GetInteractableDetails_Implementation()
{
	return 	mInteractableDetails;
}

void ABaseWeapon::Interact_Implementation(AActor* OwnerPlayer)
{
	if(UKismetSystemLibrary::DoesImplementInterface(OwnerPlayer, UPlayerInterface::StaticClass()))
	{
		IPlayerInterface::Execute_SpawnWeapon(OwnerPlayer, WeaponAsset->WeaponDetails);
		this->Destroy();
	}
}

// Playing the weapon Sound
void ABaseWeapon::PlayWeaponSound_Implementation()
{
}

// Attaching the weapon
void ABaseWeapon::AttachWeaponToPlayer_Implementation(AActor* OwnerPlayer)
{
	Server_AttachWeaponToPlayer(OwnerPlayer);

	mFireRate = mWeaponDetails.TimePerShot;
	mDamageRate = mWeaponDetails.WeaponDamage;
	mAmmo = mWeaponDetails.TotalBullets;

	UE_LOG(LogTemp, Warning, TEXT("Fire Rate: %f"), mFireRate);
}

void ABaseWeapon::Server_AttachWeaponToPlayer_Implementation(AActor* OwnerPlayer)
{
	mWeaponDetails = WeaponAsset->WeaponDetails;

	Multicast_AttachWeaponToPlayer(OwnerPlayer);

	mCollisionComponent->SetVisibility(false);
	mCollisionComponent->SetGenerateOverlapEvents(false);
	mCollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

void ABaseWeapon::Multicast_AttachWeaponToPlayer_Implementation(AActor* OwnerPlayer)
{
	mOwnerRef = OwnerPlayer;

	UMeshComponent* OwnerMesh = IPlayerInterface::Execute_GetMeshComponent(mOwnerRef);

	const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, true);

	AttachToComponent(OwnerMesh, AttachmentRules, WeaponSocket);

	mUIComponent->DestroyComponent();

	mCollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Fire
void ABaseWeapon::Fire_Implementation()
{	
	GetWorld()->GetTimerManager().SetTimer(TimeHandler, this, &ABaseWeapon::Server_Fire, mFireRate, true, mInFirstDelay);
}

void ABaseWeapon::StopFire_Implementation()
{
	Server_StopFire();
}

// Server Fire
void ABaseWeapon::Server_Fire_Implementation()
{
	if(GetOwner() == nullptr) return;

	mAmmo--;
	UE_LOG(LogTemp, Warning, TEXT("Server Ammo: %d"), mAmmo);


	if(UKismetSystemLibrary::DoesImplementInterface(GetOwner(), UPlayerInterface::StaticClass()))
	{
		UCameraComponent* FollowCam = IPlayerInterface::Execute_GetFollowCamera(GetOwner());

		FVector StartPos = FollowCam->GetComponentLocation();
		
		FVector EndPos = StartPos + (FollowCam->GetForwardVector() * TraceRange);
		
		TArray<AActor*> ActorsToIgnore = {this, GetOwner()};
		
		ActorsToIgnore.Add(GetInstigator());
		
		FHitResult hit;
		
		UKismetSystemLibrary::LineTraceSingle(GetWorld(), StartPos, EndPos, TraceChannel, false, ActorsToIgnore, EDrawDebugTrace::ForDuration, hit, true, FLinearColor::Red, FLinearColor::Green, 1.0f);

		BlueprintServer_Fire(hit);

		Client_Fire();
	}
	
	Multicast_Fire();
}
void ABaseWeapon::Client_Fire_Implementation()
{

	AActor* owner = GetOwner();
	if(UKismetSystemLibrary::DoesImplementInterface(owner, UPlayerInputInterface::StaticClass()))
	{
		IPlayerInputInterface::Execute_OnShooting(owner, mAmmo);
		UE_LOG(LogTemp, Warning, TEXT("Client Ammo: %d"), mAmmo);
	}

}


void ABaseWeapon::Server_StopFire_Implementation()
{
	Multicast_StopFire();
}

// Multicast Fire
void ABaseWeapon::Multicast_Fire_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Multicasted Ammo: %d"), mAmmo);

	Blueprint_Multicast_Fire();
}

void ABaseWeapon::Multicast_StopFire_Implementation()
{
	GetWorld()->GetTimerManager().ClearTimer(TimeHandler);
	
	Blueprint_Multicast_StopFire();

}

void ABaseWeapon::Blueprint_Multicast_Fire_Implementation()
{
}
