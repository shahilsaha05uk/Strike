// Fill out your copyright notice in the Description page of Project Settings.


#include "FlagActor.h"

#include "GameplayDebuggerTypes.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MultiplayerProject/PlayerCharacter.h"
#include "MultiplayerProject/InterfaceClasses/PlayerInterface.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AFlagActor::AFlagActor()
{
	mRoot = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = mRoot;

	mSphereComp = CreateDefaultSubobject<USphereComponent>("RangeComp");
	mSphereComp->SetupAttachment(RootComponent);
	mSphereComp->SetSphereRadius(120.0f);
	mSphereComp->OnComponentBeginOverlap.AddDynamic(this, &AFlagActor::OnSphereColliderBeginOverlap);
	mSphereComp->OnComponentEndOverlap.AddDynamic(this, &AFlagActor::OnSphereColliderEndOverlap);

	mFlagMesh = CreateDefaultSubobject<UStaticMeshComponent>("Flag Mesh");
	mFlagMesh->SetupAttachment(RootComponent);
	mFlagMesh->SetIsReplicated(true);
	
	mBoxComp = CreateDefaultSubobject<UBoxComponent>("Box Comp");
	mBoxComp->SetupAttachment(mFlagMesh);
	mBoxComp->SetBoxExtent(FVector(9.0f, 8.0f, 152.0f));
	mBoxComp->OnComponentBeginOverlap.AddDynamic(this, &AFlagActor::OnBoxColliderBeginOverlap);
	mBoxComp->OnComponentEndOverlap.AddDynamic(this, &AFlagActor::OnBoxColliderEndOverlap);

	FlagSocket = "flagSocket";

	InteractableItem = FLAG;

	bReplicates = true;
	SetReplicates(true);
	
}

void AFlagActor::BeginPlay()
{
	mBoxComp->SetVisibility(false);
	mInitialTransform = mRoot->GetComponentTransform();

	Super::BeginPlay();
}

void AFlagActor::OnSphereColliderBeginOverlap_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}

void AFlagActor::OnSphereColliderEndOverlap_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void AFlagActor::OnBoxColliderBeginOverlap_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void AFlagActor::OnBoxColliderEndOverlap_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

// New Changes
void AFlagActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFlagActor, RootFlagRef);
	DOREPLIFETIME(AFlagActor, BaseRef);
}


// Interact Item
void AFlagActor::Interact_Implementation(ACharacter* OwnerPlayer)
{
	Super::Interact_Implementation(OwnerPlayer);
	
	if(RootFlagRef == nullptr)
	{
		AFlagActor* SpawnedFlagRef = SpawnForPlayer();
		SpawnedFlagRef->RootFlagRef = this;

		Multicast_Interact(OwnerPlayer, SpawnedFlagRef, false);
		
		IPlayerInterface::Execute_OnAttachActor(OwnerPlayer, FLAG, SpawnedFlagRef);
	}
	else
	{
		Multicast_Interact(OwnerPlayer, this, true);
		
		IPlayerInterface::Execute_OnAttachActor(OwnerPlayer, FLAG, this);
	}
}

void AFlagActor::Multicast_Interact_Implementation(ACharacter* TargetActor, AFlagActor* FlagRef, bool Visibility)
{
	if(FlagRef)
	{
		Execute_AttachToPlayer(FlagRef, TargetActor);
	}

	mFlagMesh->SetVisibility(Visibility, true);
}

// Drop Item
void AFlagActor::DropItem_Implementation(ACharacter* OwnerPlayer)
{
	Multicast_DropItem(OwnerPlayer, this, BaseRef);
}

void AFlagActor::Multicast_DropItem_Implementation(ACharacter* TargetActor, AActor* FlagRef, AActor* BaseActor)
{
	if(TargetActor) Execute_DetachFromPlayer(FlagRef, TargetActor);

	if(BaseActor != nullptr)
	{
		RootFlagRef->mFlagMesh->SetVisibility(true, true);

		OnBaseCollision(BaseActor, RootFlagRef, TargetActor);	// If the Flag is within the bounds
		
		Destroy();
	}
}

// Supporting Methods
AFlagActor* AFlagActor::SpawnForPlayer_Implementation()
{
	return nullptr;
}

void AFlagActor::OnBaseCollision_Implementation(AActor* TeamBaseRef, AActor* FlagRef, ACharacter* OwningPlayer)
{
}

void AFlagActor::AttachToPlayer_Implementation(ACharacter* OwnerPlayer)
{
}

void AFlagActor::DetachFromPlayer_Implementation(ACharacter* OwnerPlayer)
{
	mBoxComp->SetVisibility(true);

	const FDetachmentTransformRules DetachRules(EDetachmentRule::KeepWorld, EDetachmentRule::KeepRelative, EDetachmentRule::KeepWorld, true);

	mRoot->DetachFromComponent(DetachRules);

	FVector tmpLoc = mFlagMesh->GetComponentLocation();
	tmpLoc.Z = mInitialTransform.GetLocation().Z;

	mRoot->SetWorldLocation(tmpLoc, false, nullptr, ETeleportType::None);
	mRoot->SetWorldRotation(FRotator::ZeroRotator, false, nullptr, ETeleportType::None);
}
