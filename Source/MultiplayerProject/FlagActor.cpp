// Fill out your copyright notice in the Description page of Project Settings.


#include "FlagActor.h"

#include "Components/WidgetComponent.h"
#include "InterfaceClasses/InputsInterface.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AFlagActor::AFlagActor()
{
	mRoot = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = mRoot;

	mRangeComp = CreateDefaultSubobject<USphereComponent>("Interact Range");
	mRangeComp->SetupAttachment(RootComponent);
	mRangeComp->SetSphereRadius(130);
	mRangeComp->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnComponentBeginOverlap);
	mRangeComp->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnComponentEndOverlap);
	
	mFlagBase = CreateDefaultSubobject<UStaticMeshComponent>("Flag Base");
	mFlagBase->SetupAttachment(RootComponent);

	mFlagWidget = CreateDefaultSubobject<UWidgetComponent>("Flag Widget");
	mFlagWidget->SetupAttachment(RootComponent);
	mFlagWidget->SetVisibility(false);

	mFlagParent = CreateDefaultSubobject<USceneComponent>("Flag Parent");
	
	mFlag = CreateDefaultSubobject<UStaticMeshComponent>("Flag");
	mFlag->SetupAttachment(mFlagParent);
	mFlag->SetVisibility(true);

}

void AFlagActor::OnComponentBeginOverlap_Implementation(UPrimitiveComponent* PrimitiveComponent, AActor* Actor,
                                                        UPrimitiveComponent* PrimitiveComponent1, int I, bool bArg, const FHitResult& HitResult)
{
	if(UKismetSystemLibrary::DoesImplementInterface(Actor, UInputsInterface::StaticClass()))
	{
		FFocusedActorDetails Details;
		Details.ActorName = GetDebugName(this);
		Details.ActorReference = this;
		Details.InteractType = FLAGCAPTURE;
		
		IInputsInterface::Execute_UpdateFocusedActor(Actor, Details);
	}
	
	mFlagWidget->SetVisibility(true);
}

void AFlagActor::OnComponentEndOverlap_Implementation(UPrimitiveComponent* PrimitiveComponent, AActor* Actor,
	UPrimitiveComponent* PrimitiveComponent1, int I)
{
	mFlagWidget->SetVisibility(false);
}

void AFlagActor::OnFlagCaptured_Implementation(USkeletalMeshComponent* MeshToAttachTo, FName SocketName)
{
	
}

void AFlagActor::OnFlagRetrieved_Implementation()
{
}
void AFlagActor::CaptureFlag_Implementation(USkeletalMeshComponent* MeshToAttachTo, FName SocketName)
{
	OnFlagCaptured(MeshToAttachTo, SocketName);
}

void AFlagActor::RetrieveFlag_Implementation()
{
	OnFlagRetrieved();
}

