// Fill out your copyright notice in the Description page of Project Settings.


#include "FlagBase.h"

#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "InterfaceClasses/InputsInterface.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AFlagBase::AFlagBase()
{
	mRoot = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = mRoot;

	mFlagBase = CreateDefaultSubobject<UStaticMeshComponent>("Flag Base");
	mFlagBase->SetupAttachment(RootComponent);

	mRangeComp = CreateDefaultSubobject<USphereComponent>("Interact Range");
	mRangeComp->SetupAttachment(RootComponent);
	mRangeComp->SetSphereRadius(130);
	mRangeComp->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnComponentBeginOverlap);
	mRangeComp->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnComponentEndOverlap);

	mFlagWidget = CreateDefaultSubobject<UWidgetComponent>("Flag Widget");
	mFlagWidget->SetupAttachment(RootComponent);
	mFlagWidget->SetVisibility(false);

	mFlagComponent = CreateDefaultSubobject<UChildActorComponent>("FlagActor");
	mFlagComponent->SetupAttachment(RootComponent);
}

void AFlagBase::CaptureFlag_Implementation(USkeletalMeshComponent* MeshToAttachTo, FName SocketName)
{
	Execute_SetFlagStatus(this ,CAPTURED);
	OnFlagCaptured(MeshToAttachTo, SocketName);
}

void AFlagBase::RetrieveFlag_Implementation()
{
	Execute_SetFlagStatus(this ,RETRIEVED);
	OnFlagRetrieved();
}
void AFlagBase::DropFlag_Implementation()
{
	Execute_SetFlagStatus(this ,DROPPED);
	OnFlagDrop();
}

EFlagStatus AFlagBase::GetFlagStatus_Implementation()
{
	return mFlagStatus;
}

void AFlagBase::SetFlagStatus_Implementation(EFlagStatus UpdatedStatus)
{
	mFlagStatus = UpdatedStatus;
}



void AFlagBase::OnComponentBeginOverlap_Implementation(UPrimitiveComponent* PrimitiveComponent, AActor* Actor,
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

void AFlagBase::OnComponentEndOverlap_Implementation(UPrimitiveComponent* PrimitiveComponent, AActor* Actor,
	UPrimitiveComponent* PrimitiveComponent1, int I)
{
	mFlagWidget->SetVisibility(false);
}

void AFlagBase::OnFlagCaptured_Implementation(USkeletalMeshComponent* MeshToAttachTo, FName SocketName)
{
	
}

void AFlagBase::OnFlagRetrieved_Implementation()
{
	
}

void AFlagBase::OnFlagDrop_Implementation()
{
	
}