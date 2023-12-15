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

	InteractableItem = WEAPON;
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
	DOREPLIFETIME(ABaseWeapon, mFireRate);
	DOREPLIFETIME(ABaseWeapon, bIsPickedUp);
	
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

}

void ABaseWeapon::OnComponentBeginOverlap_Implementation(UPrimitiveComponent* PrimitiveComponent, AActor* Actor, UPrimitiveComponent* PrimitiveComponent1, int I, bool bArg, const FHitResult& HitResult)
{
	if(UKismetSystemLibrary::DoesImplementInterface(Actor, UPlayerInterface::StaticClass()) && !bIsPickedUp)
	{
		ACharacter* c = Cast<ACharacter>(Actor);

		if(c->IsLocallyControlled())
		{
			mUIComponent->SetVisibility(true);
		}
	}
}

void ABaseWeapon::OnComponentEndOverlap_Implementation(UPrimitiveComponent* PrimitiveComponent, AActor* Actor, UPrimitiveComponent* PrimitiveComponent1, int I)
{
	if(UKismetSystemLibrary::DoesImplementInterface(Actor, UPlayerInterface::StaticClass()))
	{
		ACharacter* c = Cast<ACharacter>(Actor);

		if(c->IsLocallyControlled())
		{
			mUIComponent->SetVisibility(false);
		}
	}
}

// Virtual methods
void ABaseWeapon::Interact_Implementation(ACharacter* OwnerPlayer)
{
	if(bIsPickedUp) return;

	Super::Interact_Implementation(OwnerPlayer);
	
	if(UKismetSystemLibrary::DoesImplementInterface(OwnerPlayer, UPlayerInterface::StaticClass()))
	{
		ABaseWeapon* Weapon = SpawnForPlayer(OwnerPlayer, WeaponAsset->WeaponDetails);
		
		Weapon->bIsPickedUp = true;
		
		WeaponSetup(OwnerPlayer, Weapon);
		this->Destroy();
	}
}

void ABaseWeapon::Client_Interact_Implementation(ACharacter* OwnerPlayer, ABaseWeapon* WeaponRef, bool UIVisibility)
{
	if(WeaponRef == nullptr) return;
}

void ABaseWeapon::Multicast_Interact_Implementation(ACharacter* OwnerPlayer, ABaseWeapon* WeaponRef, bool UIVisibility)
{
	if(WeaponRef == nullptr) return;
	Execute_AttachToPlayer(WeaponRef, OwnerPlayer);
	
	WeaponRef->bIsPickedUp = true;

	mCollisionComponent->SetVisibility(false);
	mCollisionComponent->SetGenerateOverlapEvents(false);
	mCollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABaseWeapon::DropItem_Implementation(ACharacter* OwnerPlayer)
{
	Super::DropItem_Implementation(OwnerPlayer);
}

void ABaseWeapon::AttachToPlayer_Implementation(ACharacter* OwnerPlayer)
{
	
}

void ABaseWeapon::DetachFromPlayer_Implementation(ACharacter* OwnerPlayer)
{
	
}


// ----------------------------------------------------------------

// New Changes

void ABaseWeapon::WeaponSetup_Implementation(ACharacter* OwnerPlayer, ABaseWeapon* Weapon)
{
	IPlayerInterface::Execute_OnAttachActor(OwnerPlayer, WEAPON, Weapon);

	Client_Interact(OwnerPlayer, Weapon, false);
	Multicast_Interact(OwnerPlayer, Weapon, false);

	UpdateWeaponProperties(WeaponAsset->WeaponDetails, Weapon);

	Weapon->SetOwner(OwnerPlayer);
	Weapon->SetInstigator(OwnerPlayer);
	
	IPlayerInterface::Execute_OnWeaponSpawn(OwnerPlayer);
}

ABaseWeapon* ABaseWeapon::SpawnForPlayer_Implementation(ACharacter* OwnerPlayer, FWeaponDetails WeaponDetails)
{
	return nullptr;
}

void ABaseWeapon::UpdateWeaponProperties_Implementation(FWeaponDetails WeaponDetails, ABaseWeapon* WeaponRef)
{
	if(HasAuthority())
	{
		WeaponRef->mAmmo = WeaponDetails.TotalBullets;

		mParticleComponent->SetNiagaraVariableFloat("User.Duration", 0.01f);
		mParticleComponent->SetVariableFloat("User.Duration", 0.01f);

		Client_UpdateWeaponProperties(WeaponDetails, WeaponRef);
	}
}

void ABaseWeapon::Client_UpdateWeaponProperties_Implementation(FWeaponDetails WeaponDetails, ABaseWeapon* WeaponRef)
{
	WeaponRef->mFireRate = WeaponDetails.TimePerShot;
	WeaponRef->mDamageRate = WeaponDetails.WeaponDamage;
	WeaponRef->mWeaponSound = WeaponDetails.WeaponSound;
	if(WeaponRef->mWeaponSound) WeaponRef->MuzzleDuration = WeaponRef->mWeaponSound->Duration;

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
	UE_LOG(LogTemp, Warning, TEXT("Server Ammo: %d"), mAmmo);
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
	UE_LOG(LogTemp, Warning, TEXT("Client Ammo: %d"), mAmmo);

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

void ABaseWeapon::Server_AddAmmo_Implementation(int Value)
{
	mAmmo += Value;

	Request_HUDUpdate();
}

// Playing the weapon Sound
void ABaseWeapon::PlayWeaponSound_Implementation()
{
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

