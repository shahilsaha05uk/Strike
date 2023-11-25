// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MultiplayerProject/InterfaceClasses/FlagInterface.h"
#include "MultiplayerProject/InterfaceClasses/InteractableInterface.h"
#include "FlagActor.generated.h"

UCLASS()
class MULTIPLAYERPROJECT_API AFlagActor : public AActor, public IFlagInterface, public IInteractableInterface
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
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Private")
	APlayerState* mPlayerStateRef;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Private")
	TEnumAsByte<ETeam> mFlagTeam;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Private")
	FTransform mInitialTransform;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Private")
	FName FlagSocket;


	AFlagActor();

	virtual void BeginPlay() override;

	virtual void OnDropped_Implementation() override;
	virtual void OnPickedUp_Implementation(APlayerState* PlayerState) override;
	virtual ETeam GetFlagTeam_Implementation() override;

	virtual void Interact_Implementation(AActor* OwnerPlayer) override;
	virtual FInteractableDetails GetInteractableDetails_Implementation() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnSphereColliderBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnSphereColliderEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnBoxColliderBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnBoxColliderEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	virtual void ResetFlag_Implementation();
	
};

