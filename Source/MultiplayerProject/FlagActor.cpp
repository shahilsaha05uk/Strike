// Fill out your copyright notice in the Description page of Project Settings.


#include "FlagActor.h"

#include "Components/WidgetComponent.h"
#include "InterfaceClasses/InputsInterface.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AFlagActor::AFlagActor()
{
	mRoot = CreateDefaultSubobject<USceneComponent>("Flag Parent");
	
	mFlag = CreateDefaultSubobject<UStaticMeshComponent>("Flag");
	mFlag->SetupAttachment(mRoot);
	mFlag->SetVisibility(true);
}


