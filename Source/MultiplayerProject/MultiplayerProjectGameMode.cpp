// Copyright Epic Games, Inc. All Rights Reserved.

#include "MultiplayerProjectGameMode.h"

#include "GameFramework/PlayerStart.h"
#include "InterfaceClasses/ControllerInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

AMultiplayerProjectGameMode::AMultiplayerProjectGameMode()
{

}


void AMultiplayerProjectGameMode::PostLogin(APlayerController* NewPlayer)
{
	if(!mControllers.IsEmpty()) mControllers.Empty(-1);

	FindPlayerStarts();
	
	mControllers.Add(NewPlayer);

	for (int i = 0; i< mTotalPlayers; i++)
	{
		if(i == 0)
		{
			mControllers[i]->GetPawn()->Destroy();
			InitialiseController(i);
			continue;
		}
		
		mControllers[i] = GetWorld()->SpawnActor<APlayerController>();

		InitialiseController(i);
	}

	
	Super::PostLogin(NewPlayer);
}

void AMultiplayerProjectGameMode::InitialiseController_Implementation(int ControllerIndex)
{
	if(UKismetSystemLibrary::DoesImplementInterface(mControllers[ControllerIndex], UControllerInterface::StaticClass()))
	{
		IControllerInterface::Execute_Init(mControllers[ControllerIndex], playerStarts[ControllerIndex]->GetActorTransform());
	}
}

void AMultiplayerProjectGameMode::FindPlayerStarts_Implementation()
{
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), playerStarts);
}
