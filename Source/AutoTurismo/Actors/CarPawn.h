// Copyright Nat1Studios All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehiclePawn.h"
#include "CarPawn.generated.h"

class USpringArmComponent;
class UCameraComponent;
class USplineComponent;
class UMasterWidget;

/**
 * Choas wheeled vehicle pawn.
 */
UCLASS()
class AUTOTURISMO_API ACarPawn : public AWheeledVehiclePawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACarPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/**
	 * Initialize spline for vehicle to follow.
	 * 
	 * @param SplineComponent : Spline to follow.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void SetupFollowSpline(USplineComponent* SplineComponent);

	/**
	 * Reset trottle and brake input, physics and position.
	 * 
	 * @param bPosition : Whether to reset the position of the vehicle or not.
	 */
	UFUNCTION(BlueprintCallable)
		void ResetCar(bool bPosition = true);

	/**
	 * Reset the vehicle and timer.
	 */
	UFUNCTION(BlueprintCallable)
		void Restart();

protected:
	/**
	 * Update car trottle, brake and steering; can be override in blueprint.
	 */
	UFUNCTION(BlueprintNativeEvent)
		void UpdateCarMovement();

#pragma region Delegate Binding
	/**
	 * Delegate binding on throttle input is pressed.
	 */
	UFUNCTION()
		void OnThrottlePressed();
	/**
	 * Delegate binding on throttle input is released.
	 */
	UFUNCTION()
		void OnThrottleReleased();
#pragma endregion

protected:
	/* Spring arm component */
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly)
		USpringArmComponent* SpringArm;
	/* Camera component */
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly)
		UCameraComponent* Camera;

	/* Master widget class */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Widget)
		TSubclassOf<UMasterWidget> MasterWidgetClass;

	/* Spline for the vehicle to follow */
	UPROPERTY(BlueprintReadWrite, Transient, Category = Movement)
		USplineComponent* FollowSpline;
	/* Whether throttle is pressed */
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Transient, Category = Movement)
		bool bThrottle;
	/* Input key along the followed spline */
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Transient, Category = Movement)
		float SplineInputKey;
	/* Input key at the end of the spline */
	UPROPERTY(BlueprintReadWrite, Transient, Category = Movement)
		float SplineEndInputKey;

	/* Whether vehicle has reached the end of the spline */
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Transient, Category = State)
		bool bFinished;
	/* Time elapsed since the start */
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Transient, Category = State)
		float Timer;
	/* Frames elapsed since the start */
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Transient, Category = State)
		int FrameCount;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient, Category = "State|Debug")
		float SteeringInput;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "State|Debug")
		float SteeringScale = 1.0f;
};
