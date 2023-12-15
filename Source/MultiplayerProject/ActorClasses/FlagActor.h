// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MultiplayerProject/BaseClasses/BaseInteractable.h"
#include "FlagActor.generated.h"


UCLASS()
class MULTIPLAYERPROJECT_API AFlagActor : public ABaseInteractable
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Flag Components")
	class USceneComponent* mRoot;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Flag Components")
	class USphereComponent* mSphereComp;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Flag Components")
	class UBoxComponent* mBoxComp;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Flag Components")
	class UStaticMeshComponent* mFlagMesh;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Player References")
	class APlayerCharacter* PlayerRef;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Private")
	FTransform mInitialTransform;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Private")
	FName FlagSocket;
	
	AFlagActor();

	virtual void BeginPlay() override;

	virtual void Interact_Implementation(ACharacter* OwnerPlayer) override;

	UFUNCTION(Reliable, NetMulticast)
	void Multicast_Interact(ACharacter* TargetActor, AFlagActor* FlagRef, bool Visibility);

	virtual void DropItem_Implementation(ACharacter* OwnerPlayer) override;
	
	UFUNCTION(Reliable, NetMulticast)
	void Multicast_DropItem(ACharacter* TargetActor, AActor* FlagRef, AActor* BaseActor);
	
	virtual void AttachToPlayer_Implementation(ACharacter* OwnerPlayer) override;
	virtual void DetachFromPlayer_Implementation(ACharacter* OwnerPlayer) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnSphereColliderBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnSphereColliderEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnBoxColliderBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnBoxColliderEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


// New Changes

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Private")
	bool bIsInsideBase;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Private")
	AFlagActor* RootFlagRef;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Private")
	AActor* BaseRef;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	AFlagActor* SpawnForPlayer();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnBaseCollision(AActor* TeamBaseRef, AActor* FlagRef, ACharacter* OwningPlayer);
};