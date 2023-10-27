// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StructClass.h"
#include "Engine/GameInstance.h"

#include "Interfaces/OnlineSessionInterface.h"
#include "Multiplayer_GameInstance.generated.h"

UCLASS()
class MULTIPLAYERPROJECT_API UMultiplayer_GameInstance : public UGameInstance
{
	GENERATED_BODY()

private:

	
public:
	UPROPERTY(BlueprintReadOnly)
	class USession_GameInstanceComponent* SessionComp;
	UPROPERTY(BlueprintReadOnly)
	TArray<FSessionDetails> mSessionDetails;
	UPROPERTY(BlueprintReadOnly)
	FSessionDetails mCurrentSession;

	virtual void Init() override;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void CreateSession(FMatchDetails MatchDetails);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void FindSession(int MaxSearchResults, bool IsLanQuery);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void JoinSession(FSessionDetails SessionDetails);

	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnCreateSessionComplete(bool bSuccessful);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnSessionStartComplete(bool bSuccessful);
	void OnJoinSessionComplete(EOnJoinSessionCompleteResult::Type Result);
};

