// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseInteractable.h"

#include "Kismet/KismetSystemLibrary.h"
#include "MultiplayerProject/InterfaceClasses/InputsInterface.h"
#include "MultiplayerProject/InterfaceClasses/PlayerInputInterface.h"

// Sets default values
ABaseInteractable::ABaseInteractable()
{
	mRoot = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = mRoot;

	mRangeComp = CreateDefaultSubobject<USphereComponent>("Interact Range");
	mRangeComp->SetupAttachment(RootComponent);

	mRangeComp->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnComponentBeginOverlap);
	mRangeComp->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnComponentEndOverlap);
}

void ABaseInteractable::OnComponentBeginOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* Actor,
	UPrimitiveComponent* PrimitiveComponent1, int I, bool bArg, const FHitResult& HitResult)
{
	if(UKismetSystemLibrary::DoesImplementInterface(Actor, UPlayerInputInterface::StaticClass()))
	{
		OnPlayerOverlapBegin(Actor, true);
	}
}

void ABaseInteractable::OnComponentEndOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* Actor,
	UPrimitiveComponent* PrimitiveComponent1, int I)
{
	if(UKismetSystemLibrary::DoesImplementInterface(Actor, UInputsInterface::StaticClass()))
	{
		OnPlayerOverlapEnd(Actor, true);
	}
}

void ABaseInteractable::OnPlayerOverlapBegin_Implementation(AActor* Actor, bool isPlayer)
{
	if(!isPlayer) return;
	
	IInputsInterface::Execute_UpdateFocusedActor(Actor, ActorDetails);
}

void ABaseInteractable::OnPlayerOverlapEnd_Implementation(AActor* Actor, bool isPlayer)
{
	if(!isPlayer) return;
	IInputsInterface::Execute_UpdateFocusedActor(Actor, FFocusedActorDetails{});

}

void ABaseInteractable::OnInteract_Implementation()
{

}

EInteractType ABaseInteractable::GetInteractType_Implementation()
{
	return InteractableType;
}
