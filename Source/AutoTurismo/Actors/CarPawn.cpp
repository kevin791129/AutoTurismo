// Copyright Nat1Studios All Rights Reserved.

#include "CarPawn.h"
/* Components */
#include "ChaosVehicleMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SplineComponent.h"
/* Widget */
#include "AutoTurismo/UMG/MasterWidget.h"
/* Utilities */
#include "Kismet/GameplayStatics.h"
#include "AutoTurismo/Subsystems/EventSubsystem.h"
#include "AutoTurismo/Engine/MathHelper.h"
/* Other */
#include "AutoTurismo/AutoTurismo.h"

// Sets default values
ACarPawn::ACarPawn() : bThrottle(false), SplineInputKey(0.0f), SplineEndInputKey(0.0f), bFinished(true), Timer(0.0f), LapsCompleted(0), TotalLaps(1), FrameCount(0), bShadowMode(false)
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
}

// Called when the game starts or when spawned
void ACarPawn::BeginPlay()
{
	Super::BeginPlay();

	EventSubsystemAction(
		EventSubsystem->ThrottleInputPressedDelegate.AddDynamic(this, &ACarPawn::OnThrottlePressed);
		EventSubsystem->ThrottleInputReleasedDelegate.AddDynamic(this, &ACarPawn::OnThrottleReleased);
		EventSubsystem->ResetVehicleDelegate.AddDynamic(this, &ACarPawn::OnVehicleReset);
	)

	if (UUserWidget* MasterWidget = CreateWidget(GetWorld(), MasterWidgetClass))
		MasterWidget->AddToViewport();
	else
		checkNoEntry();
}

// Called every frame
void ACarPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bFinished)
		return;

	if (!FollowSpline)
	{
		UE_LOG(LogAutoTurismo, Error, TEXT("Spline to follow is invalid."))
		return;
	}

	// Increase timer
	Timer += DeltaTime;
	FrameCount++;
	EventSubsystemAction(
		if (EventSubsystem->TimerUpdateDelegate.IsBound())
			EventSubsystem->TimerUpdateDelegate.Broadcast(Timer);
	)

	// Update spline input key and check race progress
	if (UpdateSplineKey())
	{
		bFinished = true;
		ResetCar(false);
		return;
	}

	// Update mesh
	GetMesh()->SetAngularDamping(GetVehicleMovement()->IsMovingOnGround() ? 0.0f : 3.0f);

	UpdateCarMovement();
}

// Called to bind functionality to input
void ACarPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ACarPawn::SetupFollowSpline_Implementation(USplineComponent* SplineComponent, const TArray<int>& Splits, bool bLoopingTrack)
{
	if (!SplineComponent)
	{
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), FoundActors);
		for (AActor* actorIter : FoundActors)
		{
			if (actorIter->ActorHasTag(FName("Track")))
			{
				TArray<USplineComponent*> SplineComponents;
				actorIter->GetComponents(SplineComponents);
				if (SplineComponents.Num() > 0)
				{
					SplineComponent = SplineComponents[0];
					break;
				}
			}
		}
	}


	if (IsValid(SplineComponent))
	{
		FollowSpline = SplineComponent;
		bClosedLoop = bLoopingTrack;
		const int32 NumPoints = SplineComponent->GetNumberOfSplinePoints();
		TimeSplitPoints.Empty();
		NextSplitIndex = 0;
		TSet<int> TempSplits;
		TempSplits.Append(Splits);
		TempSplits.Sort([](const int& Lhs, const int& Rhs) { return Lhs < Rhs; });
		for (const int& SplitKey : TempSplits)
		{
			if (SplitKey < NumPoints)
				TimeSplitPoints.Add(SplitKey);
		}
		SplineEndInputKey = (float)(bClosedLoop ? NumPoints : NumPoints - 1);
	}
	else
	{
		UE_LOG(LogAutoTurismo, Error, TEXT("Initialize with invalid spline."))
	}
}

void ACarPawn::ResetCar(bool bPosition)
{
	GetVehicleMovement()->SetThrottleInput(0.0f);
	GetVehicleMovement()->SetBrakeInput(0.0f);
	GetMesh()->SetAllPhysicsLinearVelocity(FVector::ZeroVector);
	GetMesh()->SetAllPhysicsAngularVelocityInRadians(FVector::ZeroVector);

	if (bPosition)
	{
		if (!FollowSpline)
		{
			UE_LOG(LogAutoTurismo, Warning, TEXT("Spline component is null"))
			return;
		}
		const FTransform SplineTransform = FollowSpline->GetTransformAtSplineInputKey(SplineInputKey, ESplineCoordinateSpace::World);
		SetActorTransform(SplineTransform, false, nullptr, ETeleportType::TeleportPhysics);
	}
}

void ACarPawn::RestartRace(int NumLaps)
{
	SplineInputKey = KINDA_SMALL_NUMBER;
	bFinished = false;
	Timer = 0.0f;
	LapsCompleted = 0;
	TotalLaps = NumLaps;
	NextSplitIndex = 0;
	TimeSplits = FRaceSplit(NumLaps, TimeSplitPoints.Num() + 1);
	FrameCount = 0;
	ThrottleInputSave.Empty(18000);
	bShadowMode = false;
	EventSubsystemAction(
		if (EventSubsystem->TimerResetDelegate.IsBound())
			EventSubsystem->TimerResetDelegate.Broadcast();
	)
	ResetCar();
}

void ACarPawn::StartShadowReplay()
{
	if (!bFinished || ThrottleInputSave.Num() <= 0)
	{
		UE_LOG(LogAutoTurismo, Warning, TEXT("Attempting to replay shadow at invalid time."))
		return;
	}

	SplineInputKey = KINDA_SMALL_NUMBER;
	bFinished = false;
	Timer = 0.0f;
	LapsCompleted = 0;
	FrameCount = 0;
	bShadowMode = false;
	ResetCar();
}

void ACarPawn::SetDriveSettings(const FDriveSettings& InSettings)
{
	DriveSettings = InSettings;
}

bool ACarPawn::UpdateSplineKey()
{
	// Around input key 0 in a closed spline loop there is a weird behavior where closest key 
	// can be before the end (LAST_KEY - SMALL_NUM) instead of after start (0.0f + SMALL_NUM).
	// This will cause instant lap finish.
	const float NewSplineInputKey = FollowSpline->FindInputKeyClosestToWorldLocation(GetActorLocation());
	const float Dist = NewSplineInputKey >= SplineInputKey ? NewSplineInputKey - SplineInputKey : FMath::Abs(SplineEndInputKey - SplineInputKey) + NewSplineInputKey;
	if (Dist >= 0.1f)
		return false;

	if (NewSplineInputKey < SplineInputKey)
	{
		if (!bShadowMode)
		{
			const float SplitTime = TimeSplits.AddSplit(LapsCompleted, Timer);
			const float LapTime = TimeSplits.GetLapTime(LapsCompleted);
			UE_LOG(LogAutoTurismo, Log, TEXT("Split: %f - Lap: %f (%f)"), SplitTime, LapTime, Timer)
			EventSubsystemAction(
				if (EventSubsystem->TimeSplitDelegate.IsBound())
					EventSubsystem->TimeSplitDelegate.Broadcast(Timer);
				if (EventSubsystem->LapFinishDelegate.IsBound())
					EventSubsystem->LapFinishDelegate.Broadcast(LapTime);
			)
		}
		NextSplitIndex = 0;
		LapsCompleted++;
	}
	else if (TimeSplitPoints.Num() > NextSplitIndex && SplineInputKey >= TimeSplitPoints[NextSplitIndex])
	{
		if (!bShadowMode)
		{
			const float SplitTime = TimeSplits.AddSplit(LapsCompleted, Timer);
			UE_LOG(LogAutoTurismo, Log, TEXT("Split: %f (%f)"), SplitTime, Timer)
			EventSubsystemAction(
				if (EventSubsystem->TimeSplitDelegate.IsBound())
					EventSubsystem->TimeSplitDelegate.Broadcast(Timer);
			)
		}
		NextSplitIndex++;
	}
	SplineInputKey = NewSplineInputKey;
	return LapsCompleted >= TotalLaps;
}

void ACarPawn::UpdateCarMovement()
{
	if (!bShadowMode)
	{
		ThrottleInputSave.Add(bThrottle);
	}
	else if (FrameCount >= ThrottleInputSave.Num())
	{
		UE_LOG(LogAutoTurismo, Warning, TEXT("Replaying shadow throttle input out of bounds"))
		return;
	}

	// Throttle and brake input
	if (!bShadowMode ? bThrottle : ThrottleInputSave[FrameCount])
	{
		GetVehicleMovement()->SetThrottleInput(1.0f);
		GetVehicleMovement()->SetBrakeInput(0.0f);
	}
	else
	{
		GetVehicleMovement()->SetThrottleInput(0.0f);
		GetVehicleMovement()->SetBrakeInput(GetVehicleMovement()->GetForwardSpeed() > 0.0f ? DriveSettings.BrakeInput : 0.0f);
	}

	// Steering based on spline direction
	const FVector SplineDir = FollowSpline->GetDirectionAtSplineInputKey(SplineInputKey, ESplineCoordinateSpace::World).GetSafeNormal();
	const FVector ForwardDir = GetActorForwardVector().GetSafeNormal();
	const float ForwardAngle = UMathHelper::GetAngleBetweenVectors(ForwardDir, SplineDir, FVector::UpVector);

	// Steering based on position
	float PeakKey = SplineInputKey + DriveSettings.PeakDist;
	if (PeakKey > SplineEndInputKey)
	{
		if (bClosedLoop)
			PeakKey -= SplineEndInputKey;
		else
			PeakKey = SplineEndInputKey;
	}
	FVector PositionDiff = GetActorLocation() - FollowSpline->GetLocationAtSplineInputKey(SplineInputKey, ESplineCoordinateSpace::World);
	PositionDiff.Z = 0;
	const float PositionDistSquared = PositionDiff.SquaredLength();
	FVector PositionDir = FollowSpline->GetLocationAtSplineInputKey(PeakKey, ESplineCoordinateSpace::World) - GetActorLocation();
	PositionDir.Z = 0;
	const float PositionScale = FMath::Lerp(0.0f, 1.0f, FMath::Clamp(PositionDistSquared / DriveSettings.DistanceThreshold, 0.0f, 1.0f));
	const float PositionAngle = UMathHelper::GetAngleBetweenVectors(ForwardDir, PositionDir.GetSafeNormal(), FVector::UpVector) * PositionScale;

	// Steering input
	const float SteeringInput = FMath::RadiansToDegrees(ForwardAngle * DriveSettings.ForwardMultiplier + PositionAngle * DriveSettings.PositionMultiplier) * DriveSettings.SteeringMultiplier;
	GetVehicleMovement()->SetSteeringInput(SteeringInput);

#if WITH_EDITORONLY_DATA 
	DebugSplineDir = SplineDir;
	DebugForwardDir = ForwardDir;
	DebugForwardAngle = ForwardAngle;
	DebugPositionDistSquared = PositionDistSquared;
	DebugPositionDir = PositionDir;
	DebugPositionScale = PositionScale;
	DebugPositionAngle = PositionAngle;
	DebugSteeringInput = SteeringInput;
	if (bDebugDraw)
	{
		FVector SplineLocation = FVector::ZeroVector;
		SplineLocation = FollowSpline->GetLocationAtSplineInputKey(SplineInputKey, ESplineCoordinateSpace::World);
		DrawDebugPoint(GetWorld(), SplineLocation + FVector::UpVector * 200.0f, 10.0f, FColor::Red, false, 1.0f);
		SplineLocation = FollowSpline->GetLocationAtSplineInputKey(PeakKey, ESplineCoordinateSpace::World);
		DrawDebugPoint(GetWorld(), SplineLocation + FVector::UpVector * 200.0f, 10.0f, FColor::Green, false, 1.0f);
	}
#endif
}

#pragma region Delegate Binding
void ACarPawn::OnThrottlePressed()
{
	bThrottle = true;
}

void ACarPawn::OnThrottleReleased()
{
	bThrottle = false;
}

void ACarPawn::OnVehicleReset()
{
	if (bFinished)
		RestartRace(TotalLaps);
	else
		ResetCar();
}
#pragma endregion

