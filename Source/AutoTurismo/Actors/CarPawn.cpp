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
/* Other */
#include "AutoTurismo/AutoTurismo.h"

// Sets default values
ACarPawn::ACarPawn() : bThrottle(false), SplineInputKey(0.0f), SplineEndInputKey(0.0f), bFinished(true), Timer(0.0f), FrameCount(0), bShadowMode(false)
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
		EventSubsystem->ResetVehicleDelegate.AddDynamic(this, &ACarPawn::Restart);
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

	// Update spline input key
	SplineInputKey = FollowSpline->FindInputKeyClosestToWorldLocation(GetActorLocation());
	if (SplineInputKey >= SplineEndInputKey)
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

void ACarPawn::SetupFollowSpline_Implementation(USplineComponent* SplineComponent)
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
		const int32 NumPoints = SplineComponent->GetNumberOfSplinePoints();
		SplineEndInputKey = (float)(NumPoints - 1);
		Restart();
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
		// TODO: Tiggered once on car spawn before spline component is set. 
		//       This should not be the case and needs investigation.
		if (!FollowSpline)
		{
			UE_LOG(LogAutoTurismo, Warning, TEXT("Spline component is null"))
			return;
		}
		const FTransform SplineTransform = FollowSpline->GetTransformAtSplineInputKey(SplineInputKey, ESplineCoordinateSpace::World);
		SetActorTransform(SplineTransform, false, nullptr, ETeleportType::TeleportPhysics);
	}
}

void ACarPawn::Restart()
{
	if (bFinished)
	{
		ThrottleInputSave.Empty(18000);
		SplineInputKey = 0.0f;
		bFinished = false;
		Timer = 0.0f;
		FrameCount = 0;
		bShadowMode = false;
		EventSubsystemAction(
			if (EventSubsystem->TimerUpdateDelegate.IsBound())
				EventSubsystem->TimerUpdateDelegate.Broadcast(Timer);
		)
	}
	
	ResetCar();
}

void ACarPawn::StartShadowReplay()
{
	if (!bFinished || ThrottleInputSave.Num() <= 0)
	{
		UE_LOG(LogAutoTurismo, Warning, TEXT("Attempting to replay shadow at invalid time."))
		return;
	}

	SplineInputKey = 0.0f;
	bFinished = false;
	Timer = 0.0f;
	FrameCount = 0;
	bShadowMode = true;
	EventSubsystemAction(
		if (EventSubsystem->TimerUpdateDelegate.IsBound())
			EventSubsystem->TimerUpdateDelegate.Broadcast(Timer);
	)
	ResetCar();
}

void ACarPawn::UpdateCarMovement_Implementation()
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

	if (!bShadowMode ? bThrottle : ThrottleInputSave[FrameCount])
	{
		GetVehicleMovement()->SetThrottleInput(1.0f);
		GetVehicleMovement()->SetBrakeInput(0.0f);
	}
	else
	{
		GetVehicleMovement()->SetThrottleInput(0.0f);
		GetVehicleMovement()->SetBrakeInput(GetVehicleMovement()->GetForwardSpeed() > 0.0f ? 1.0f : 0.0f);
	}
	const FVector SplineDirection = FollowSpline->GetDirectionAtSplineInputKey(SplineInputKey, ESplineCoordinateSpace::World).GetSafeNormal();
	const FVector ForwardDirection = GetActorForwardVector().GetSafeNormal();
	// Cheat to know whether we need to turn right/left
	const float AngleSign = FVector::CrossProduct(ForwardDirection, SplineDirection).Z >= 0.0f ? 1.0f : -1.0f;
	const float Angle = FMath::Acos(FVector::DotProduct(SplineDirection, ForwardDirection)) * AngleSign;
	SteeringInput = FMath::RadiansToDegrees(Angle);
	GetVehicleMovement()->SetSteeringInput(SteeringInput * SteeringScale);
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
#pragma endregion

