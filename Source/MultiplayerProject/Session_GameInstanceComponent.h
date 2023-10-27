// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StructClass.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Session_GameInstanceComponent.generated.h"

// These delegates will be used by the other classes

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(F_MP_OnCreateSessionComplete, bool, Successful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(F_MP_OnUpdateSessionComplete, bool, Successful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(F_MP_OnStartSessionComplete, bool, Successful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(F_MP_OnEndSessionComplete, bool, Successful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(F_MP_OnDestroySessionComplete, bool, Successful);
DECLARE_MULTICAST_DELEGATE_TwoParams(F_MP_OnFindSessionsComplete, const TArray<FSessionDetails>& SessionResults, bool Successful);
DECLARE_MULTICAST_DELEGATE_OneParam(F_MP_OnJoinSessionComplete, EOnJoinSessionCompleteResult::Type Result);

// ------

UCLASS()
class MULTIPLAYERPROJECT_API USession_GameInstanceComponent : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	USession_GameInstanceComponent();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SessionProperties")
	FMatchDetails mMatchDetails;
	
	
	IOnlineSessionPtr mSessionInterface;
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	// These methods will be called and used by the other classes
	UFUNCTION(BlueprintCallable)
	void CreateSession(FMatchDetails MatchDetails);
	UFUNCTION(BlueprintCallable)
	void UpdateSession();
	UFUNCTION(BlueprintCallable)
	void StartSession();
	UFUNCTION(BlueprintCallable)
	void EndSession();
	UFUNCTION(BlueprintCallable)
	void DestroySession();
	UFUNCTION(BlueprintCallable)
	void FindSessions(int MaxSearchResults = 10, bool IsLanQuery = false);

	void JoinSession(const FOnlineSessionSearchResult& SessionResult);

	// These delegates will be used by the implementing classes
	F_MP_OnCreateSessionComplete Event_OnCreateSessionComplete;
	F_MP_OnUpdateSessionComplete Event_OnUpdateSessionComplete;
	F_MP_OnStartSessionComplete Event_OnStartSessionComplete;
	F_MP_OnEndSessionComplete Event_OnEndSessionComplete;
	F_MP_OnDestroySessionComplete Event_OnDestroySessionComplete;
	F_MP_OnFindSessionsComplete Event_OnFindSessionsComplete;
	F_MP_OnJoinSessionComplete Event_OnJoinGameSessionComplete;	

protected:
	void OnCreateSessionCompleted(FName SessionName, bool Successful);
	void OnUpdateSessionCompleted(FName SessionName, bool Successful);
	void OnStartSessionCompleted(FName SessionName, bool Successful);
	void OnEndSessionCompleted(FName SessionName, bool Successful);
	void OnDestroySessionCompleted(FName SessionName, bool Successful);
	void OnFindSessionsCompleted(bool Successful);
	void OnJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	bool TryTravelToCurrentSession();
	
private:
	
	

	FOnCreateSessionCompleteDelegate CompleteDelegate_CreateSession;
	FDelegateHandle Handle_CreateSessionCompleteDelegate;
	TSharedPtr<FOnlineSessionSettings> LastSessionSettings;

	FOnUpdateSessionCompleteDelegate CompleteDelegate_UpdateSession;
	FDelegateHandle Handle_UpdateSessionCompleteDelegate;
	
	FOnStartSessionCompleteDelegate CompleteDelegate_StartSession;
	FDelegateHandle Handle_StartSessionCompleteDelegate;
	
	FOnEndSessionCompleteDelegate CompleteDelegate_EndSession;
	FDelegateHandle Handle_EndSessionCompleteDelegate;
	
	FOnDestroySessionCompleteDelegate CompleteDelegate_DestroySession;
	FDelegateHandle Handle_DestroySessionCompleteDelegate;
	
	FOnFindSessionsCompleteDelegate CompleteDelegate_FindSession;
	FDelegateHandle Handle_FindSessionsCompleteDelegate;
	TSharedPtr<FOnlineSessionSearch> LastSessionSearch;
	
	FOnJoinSessionCompleteDelegate CompleteDelegate_JoinSession;
	FDelegateHandle Handle_JoinSessionCompleteDelegate;

	
};
