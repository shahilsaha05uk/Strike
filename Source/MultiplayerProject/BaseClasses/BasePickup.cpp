// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePickup.h"

#include "Components/SphereComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MultiplayerProject/InterfaceClasses/InputsInterface.h"
#include "MultiplayerProject/InterfaceClasses/PlayerInputInterface.h"


// Sets default values
ABasePickup::ABasePickup()
{
	mRoot = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = mRoot;

	mRangeComp = CreateDefaultSubobject<USphereComponent>("Interact Range");
	mRangeComp->SetupAttachment(RootComponent);

	mRangeComp->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnComponentBeginOverlap);
	mRangeComp->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnComponentEndOverlap);

}

void ABasePickup::OnComponentBeginOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* Actor,
	UPrimitiveComponent* PrimitiveComponent1, int I, bool bArg, const FHitResult& HitResult)
{
	if(UKismetSystemLibrary::DoesImplementInterface(Actor, UPlayerInputInterface::StaticClass()))
	{
		OnPlayerOverlapBegin(Actor, true);
	}

}

void ABasePickup::OnComponentEndOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* Actor,
	UPrimitiveComponent* PrimitiveComponent1, int I)
{
	if(UKismetSystemLibrary::DoesImplementInterface(Actor, UInputsInterface::StaticClass()))
	{
		OnPlayerOverlapEnd(Actor, true);
	}

}

void ABasePickup::OnPlayerOverlapBegin_Implementation(AActor* Actor, bool isPlayer)
{
	if(!isPlayer) return;
	
	IInputsInterface::Execute_UpdateFocusedActor(Actor, ActorDetails);

}

void ABasePickup::OnPlayerOverlapEnd_Implementation(AActor* Actor, bool isPlayer)
{
	if(!isPlayer) return;
	IInputsInterface::Execute_UpdateFocusedActor(Actor, FFocusedActorDetails{});

}

EInteractType ABasePickup::GetInteractType_Implementation()
{
	return PICKUP;
}

void ABasePickup::OnPickup_Implementation()
{
	
}
