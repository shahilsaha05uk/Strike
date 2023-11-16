// Fill out your copyright notice in the Description page of Project Settings.


#include "FlagActor.h"

// Sets default values
AFlagActor::AFlagActor()
{
	mFlagBase = CreateDefaultSubobject<UStaticMeshComponent>("Flag Base");
	mFlagBase->SetupAttachment(RootComponent);

	mRangeComp->SetSphereRadius(128.0f);

	const FAttachmentTransformRules Rules = FAttachmentTransformRules::KeepWorldTransform;
	
	mFlag = CreateDefaultSubobject<UStaticMeshComponent>("Flag");
	mFlag->SetupAttachment(RootComponent);
	mFlag->SetVisibility(true);


	InteractableType = PICKUP;
	
	ActorDetails.ActorName = GetName();
	ActorDetails.InteractType = InteractableType;
	ActorDetails.ActorReference = this;

}

void AFlagActor::AttachFlag()
{
	if(mFlag->IsVisible())
	{
		mFlag->SetVisibility(false);
	}
	else
	{
		mFlag->SetVisibility(true);
	}
}

void AFlagActor::OnPlayerOverlapBegin_Implementation(AActor* Actor, bool isPlayer)
{
	Super::OnPlayerOverlapBegin_Implementation(Actor, isPlayer);
}

void AFlagActor::OnPlayerOverlapEnd_Implementation(AActor* Actor, bool isPlayer)
{
	Super::OnPlayerOverlapEnd_Implementation(Actor, isPlayer);
}
