// Fill out your copyright notice in the Description page of Project Settings.


#include "FlagActor.h"

#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MultiplayerProject/InterfaceClasses/PlayerInterface.h"

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

	mBoxComp = CreateDefaultSubobject<UBoxComponent>("Box Comp");
	mBoxComp->SetupAttachment(mFlagMesh);
	mBoxComp->SetBoxExtent(FVector(9.0f, 8.0f, 152.0f));
	mBoxComp->OnComponentBeginOverlap.AddDynamic(this, &AFlagActor::OnBoxColliderBeginOverlap);
	mBoxComp->OnComponentEndOverlap.AddDynamic(this, &AFlagActor::OnBoxColliderEndOverlap);

	FlagSocket = "flagSocket";
}

void AFlagActor::BeginPlay()
{
	mBoxComp->SetVisibility(false);
	
	mInitialTransform = mRoot->GetComponentTransform();

	Super::BeginPlay();
}

void AFlagActor::OnDropped_Implementation()
{
	mBoxComp->SetVisibility(true);

	const FDetachmentTransformRules DetachRules(EDetachmentRule::KeepWorld, EDetachmentRule::KeepRelative, EDetachmentRule::KeepWorld, true);

	mRoot->DetachFromComponent(DetachRules);

	FVector tmpLoc = mFlagMesh->GetComponentLocation();
	tmpLoc.Z = mInitialTransform.GetLocation().Z;

	mRoot->SetWorldLocation(tmpLoc, false, nullptr, ETeleportType::None);
	mRoot->SetWorldRotation(FRotator::ZeroRotator, false, nullptr, ETeleportType::None);
}

void AFlagActor::OnPickedUp_Implementation(APlayerState* PlayerState)
{
	mPlayerStateRef = PlayerState;
}

ETeam AFlagActor::GetFlagTeam_Implementation()
{
	return mFlagTeam;
}

void AFlagActor::Interact_Implementation(AActor* OwnerPlayer)
{
	if(UKismetSystemLibrary::DoesImplementInterface(OwnerPlayer, UPlayerInterface::StaticClass()))
	{
		IPlayerInterface::Execute_FlagSpawner(OwnerPlayer, this);
	}
}

FInteractableDetails AFlagActor::GetInteractableDetails_Implementation()
{
	return {};
}

void AFlagActor::ResetFlag_Implementation()
{
	mRoot->SetWorldTransform(mInitialTransform);
}


void AFlagActor::OnSphereColliderBeginOverlap_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}

void AFlagActor::OnSphereColliderEndOverlap_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void AFlagActor::OnBoxColliderBeginOverlap_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void AFlagActor::OnBoxColliderEndOverlap_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

