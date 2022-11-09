// Copyright Nat1Studios All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehiclePawn.h"
#include "AutoTurismo/Structs/TimeSplit.h"
#include "AutoTurismo/Structs/DriveSettings.h"
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
	 * @param Splits : Spline points to create time splits.
	 * @param bLoopingTrack : True if the track is a closed loop, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void SetupFollowSpline(USplineComponent* SplineComponent, const TArray<int>& Splits, bool bLoopingTrack = true);
	virtual void SetupFollowSpline_Implementation(USplineComponent* SplineComponent, const TArray<int>& Splits, bool bLoopingTrack);

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
		void RestartRace(int NumLaps = 1);

	/**
	 * Replay recorded race as shadow car.
	 */
	UFUNCTION(BlueprintCallable)
		void StartShadowReplay();

	/**
	 * Set drive settings.
	 * 
	 * @param InSettings : New drive settings.
	 */
	UFUNCTION(BlueprintCallable)
		void SetDriveSettings(const FDriveSettings& InSettings);

protected:
	/**
	 * Update spline key and time splits.
	 *
	 * @return : True if race is completed, false otherwise.
	 */
	bool UpdateSplineKey();

	/**
	 * Update car trottle, brake and steering; can be override in blueprint.
	 * 
	 * TODO : Heavily dependent on frame rate as it is currently being called in Tick.
	 */
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

	/**
	 * Delegate binding on vehicle is reset.
	 */
	UFUNCTION()
		void OnVehicleReset();
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
	UPROPERTY(BlueprintReadWrite, Transient, Category = Track)
		USplineComponent* FollowSpline;
	/* Whether spline is a loop */
	UPROPERTY(BlueprintReadWrite, Transient, Category = Track)
		bool bClosedLoop;

	/* Drive settings */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Drive Settings", meta = (ShowOnlyInnerProperties))
		FDriveSettings DriveSettings;

	/* Whether throttle is pressed */
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Transient, Category = State)
		bool bThrottle;
	/* Input key along the followed spline */
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Transient, Category = State)
		float SplineInputKey;
	/* Input key at the end of the spline */
	UPROPERTY(BlueprintReadWrite, Transient, Category = Movement)
		float SplineEndInputKey;
	/* Whether race is finished or laps completed equals total laps */
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Transient, Category = State)
		bool bFinished;
	/* Time elapsed since the start */
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Transient, Category = State)
		float Timer;
	/* Number of laps completed */
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Transient, Category = State)
		int LapsCompleted;
	/* Total number of laps that needs to be completed to finish the race */
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Transient, Category = State)
		int TotalLaps;

	/* Spline points where timer splits */
	UPROPERTY(BlueprintReadWrite, Transient, Category = "State|Time Split")
		TArray<int> TimeSplitPoints;
	/* Next spline point for timer split */
	UPROPERTY(BlueprintReadWrite, Transient, Category = "State|Time Split")
		int NextSplitIndex;
	/* Race time splits */
	UPROPERTY(BlueprintReadWrite, Transient, Category = "State|Time Split")
		FRaceSplit TimeSplits;

	/* Frames elapsed since the start */
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, AdvancedDisplay, Transient, Category = State)
		int FrameCount;
	/* Throttle input save data */
	TBitArray<> ThrottleInputSave;
	/* Replay recorded race as shadow car */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, AdvancedDisplay, Transient, Category = State)
		bool bShadowMode;

private:
#if WITH_EDITORONLY_DATA
	/* Steering input visible only for debug only */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient, Category = "State|Debug", meta = (AllowPrivateAccess = true, DisplayName = "Steering Input"))
		float DebugSteeringInput;
	/* Forward direction visible only for debug only */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient, Category = "State|Debug", meta = (AllowPrivateAccess = true, DisplayName = "Forward Direction"))
		FVector DebugForwardDir;
	/* Spline direction visible only for debug only */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient, Category = "State|Debug", meta = (AllowPrivateAccess = true, DisplayName = "Spline Direction"))
		FVector DebugSplineDir;
	/* Forward angle visible only for debug only */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient, Category = "State|Debug", meta = (AllowPrivateAccess = true, DisplayName = "Forward Angle"))
		float DebugForwardAngle;
	/* Position distance squared visible only for debug only */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient, Category = "State|Debug", meta = (AllowPrivateAccess = true, DisplayName = "Position Distance Squared"))
		float DebugPositionDistSquared;
	/* Position direction visible only for debug only */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient, Category = "State|Debug", meta = (AllowPrivateAccess = true, DisplayName = "Position Direction"))
		FVector DebugPositionDir;
	/* Position scale visible only for debug only */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient, Category = "State|Debug", meta = (AllowPrivateAccess = true, DisplayName = "Position Scale"))
		float DebugPositionScale;
	/* Position angle visible only for debug only */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient, Category = "State|Debug", meta = (AllowPrivateAccess = true, DisplayName = "Position Angle"))
		float DebugPositionAngle;
	/* Whether to draw spline key points only for debug only */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "State|Debug", meta = (AllowPrivateAccess = true, DisplayName = "Draw Key Points"))
		bool bDebugDraw = false;
#endif
};
