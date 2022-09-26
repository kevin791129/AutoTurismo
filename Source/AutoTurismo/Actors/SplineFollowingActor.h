// Copyright Nat1Studios All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SplineFollowingActor.generated.h"

class USplineComponent;
class UStaticMeshComponent;
class UCameraComponent;

UCLASS()
class AUTOTURISMO_API ASplineFollowingActor : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASplineFollowingActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	/**
	 * Set actor transform to point given distance along on spline.
	 * 
	 * @param Dist: Distance along spline.
	 */
	UFUNCTION(BlueprintCallable)
		void FollowSpline(float Dist);

	/**
	 * Delegate binding on follow input pressed.
	 */
	UFUNCTION()
		void StartFollowing();
	/**
	 * Delegate binding on follow input released.
	 */
	UFUNCTION()
		void StopFollowing();

protected:
	/* Mesh component*/
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		UStaticMeshComponent* Mesh;
	/* Camera component*/
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		UCameraComponent* Camera;

	/* Spline actor class to follow*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TSubclassOf<AActor> SplineActorClass;
	/* Spline actor tag*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FName SplineTag;

	/* Spline component to follow along*/
	UPROPERTY(BlueprintReadWrite)
		USplineComponent* SplineComponent;
	/* Current spline distance*/
	UPROPERTY(BlueprintReadWrite)
		float CurrentDist;
	/* Max spline distance*/
	UPROPERTY(BlueprintReadWrite)
		float MaxDist;

	/* Following key down*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool bFollowing;
	/* Follow speed*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float Speed;
};
