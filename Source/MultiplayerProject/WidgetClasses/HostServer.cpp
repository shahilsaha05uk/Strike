// Fill out your copyright notice in the Description page of Project Settings.


#include "HostServer.h"

#include "Components/Button.h"

void UHostServer::NativeConstruct()
{

	Super::NativeConstruct();

	if(btnHost) btnHost->OnClicked.AddDynamic(this, &UHostServer::OnHost);
}

void UHostServer::OnHost_Implementation()
{
	
}
