// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWeapon.h"

#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MultiplayerProject/PlayerCharacter.h"
#include "MultiplayerProject/DataAssetClasses/DA_WeaponDetails.h"
#include "MultiplayerProject/InterfaceClasses/PlayerInputInterface.h"
#include "Net/UnrealNetwork.h"

ABaseWeapon::ABaseWeapon()
{
	mWeaponSocket = "weapon_r";
	
	mCollisionComponent = CreateDefaultSubobject<USphereComponent>("CollisionComponent");

	RootComponent = mCollisionComponent;

	mWeapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	mWeapon->SetupAttachment(RootComponent);
	
	mCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ABaseWeapon::OnComponentBeginOverlap);
	mCollisionComponent->OnComponentEndOverlap.AddDynamic(this, &ABaseWeapon::OnComponentEndOverlap);

	mParticleComponent = CreateDefaultSubobject<UNiagaraComponent>("ParticleComp");
	mParticleComponent->SetupAttachment(mWeapon, mMuzzleSocket);
	
	mUIComponent = CreateDefaultSubobject<UWidgetComponent>("Popup Component");
	mUIComponent->SetupAttachment(mCollisionComponent);
	mUIComponent->SetDrawAtDesiredSize(true);
	mUIComponent->SetWidgetClass(mWidgetClass);
	mUIComponent->SetTickMode(ETickMode::Automatic);
	mUIComponent->SetVisibility(false);
	
	bReplicates = true;

	mSpread = 800.0f;

	TraceRange = 20000.0f;
	
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
	DOREPLIFETIME(ABaseWeapon, mWeaponSound);
	
}

void ABaseWeapon::Request_HUDUpdate_Implementation()
{
	AActor* owner = GetOwner();
	if(UKismetSystemLibrary::DoesImplementInterface(owner, UPlayerInputInterface::StaticClass()))
	{
		IPlayerInputInterface::Execute_OnShooting(owner, mAmmo);
	}
}

void ABaseWeapon::Init_Implementation()
{
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

void ABaseWeapon::Server_AddAmmo_Implementation(int Value)
{
	mAmmo += Value;

	Request_HUDUpdate();

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
	mWeaponSound = mWeaponDetails.WeaponSound;
	MuzzleDuration = mWeaponSound->Duration;
}

void ABaseWeapon::Server_AttachWeaponToPlayer_Implementation(AActor* OwnerPlayer)
{
	mWeaponDetails = WeaponAsset->WeaponDetails;
	
	mAmmo = mWeaponDetails.TotalBullets;
	
	Client_AttachWeaponToPlayer();
	
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

	AttachToComponent(OwnerMesh, AttachmentRules, mWeaponSocket);

	mUIComponent->DestroyComponent();

	mCollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	mParticleComponent->SetNiagaraVariableFloat("User.Duration", 0.01f);
	mParticleComponent->SetVariableFloat("User.Duration", 0.01f);
}

void ABaseWeapon::Client_AttachWeaponToPlayer_Implementation()
{
	Request_HUDUpdate();
}

// Fire
void ABaseWeapon::Fire_Implementation()
{
	GetWorld()->GetTimerManager().SetTimer(TimeHandler, this, &ABaseWeapon::Server_Fire, mFireRate, true, 0);
}

void ABaseWeapon::StopFire_Implementation()
{
	Server_StopFire();
}

// Server Fire
void ABaseWeapon::Server_Fire_Implementation()
{
	if(GetOwner() == nullptr) return;
	if(mAmmo <= 0)
	{
		Request_HUDUpdate();
		return;
	}

	if(UKismetSystemLibrary::DoesImplementInterface(GetOwner(), UPlayerInterface::StaticClass()))
	{
		FHitResult hit;
		
		UCameraComponent* FollowCam = IPlayerInterface::Execute_GetFollowCamera(GetOwner());

		FVector StartPos = FollowCam->GetComponentLocation();
		
		FVector EndPos = SpreadTrace(StartPos + (FollowCam->GetForwardVector() * TraceRange));

		TArray<AActor*> ActorsToIgnore = {this, GetOwner()};
		
		ActorsToIgnore.Add(GetInstigator());
		
		UKismetSystemLibrary::LineTraceSingle(GetWorld(), StartPos, EndPos, TraceChannel, false, ActorsToIgnore, EDrawDebugTrace::None, hit, true);

		BlueprintServer_Fire(hit);

		Client_Fire();
		mAmmo--;
		Multicast_Fire(hit);
	}
}

void ABaseWeapon::Client_Fire_Implementation()
{
	Request_HUDUpdate();
}

void ABaseWeapon::Server_StopFire_Implementation()
{
	Multicast_StopFire();
}

// Multicast Fire
void ABaseWeapon::Multicast_Fire_Implementation(FHitResult hit)
{
	Blueprint_Multicast_Fire(hit);
}

void ABaseWeapon::Multicast_StopFire_Implementation()
{
	GetWorld()->GetTimerManager().ClearTimer(TimeHandler);
	
	Blueprint_Multicast_StopFire();
}

FVector ABaseWeapon::SpreadTrace(FVector InputTrace)
{
	float spread = mSpread * -1;

	float x = UKismetMathLibrary::RandomFloatInRange(spread, mSpread);
	float y = UKismetMathLibrary::RandomFloatInRange(spread, mSpread);
	float z = UKismetMathLibrary::RandomFloatInRange(spread, mSpread);

	const FVector value = FVector(x,y,z) + InputTrace;

	return value;
}

