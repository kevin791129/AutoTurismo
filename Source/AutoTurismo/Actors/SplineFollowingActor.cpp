// Copyright Nat1Studios All Rights Reserved.


#include "SplineFollowingActor.h"
#include "Components/StaticMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SplineComponent.h"
#include "Kismet/GameplayStatics.h"
#include "AutoTurismo/Subsystems/EventSubsystem.h"
#include "AutoTurismo/AutoTurismo.h"

// Sets default values
ASplineFollowingActor::ASplineFollowingActor() : CurrentDist(0.0f), MaxDist(0.0f), bFollowing(false), Speed(0.0f)
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(RootComponent);
	Camera->SetRelativeLocation(FVector(0, 0, 2000));
	Camera->SetRelativeRotation(FRotator(-90, 0, 0));
}

// Called when the game starts or when spawned
void ASplineFollowingActor::BeginPlay()
{
	Super::BeginPlay();

	EventSubsystemAction(
		EventSubsystem->ThrottleInputPressedDelegate.AddDynamic(this, &ASplineFollowingActor::StartFollowing);
		EventSubsystem->ThrottleInputReleasedDelegate.AddDynamic(this, &ASplineFollowingActor::StopFollowing);
	)
	
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), SplineActorClass, FoundActors);
	for (AActor* actorIter : FoundActors)
	{
		if (actorIter->ActorHasTag(SplineTag))
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

	if (IsValid(SplineComponent))
	{
		const int32 NumPoints = SplineComponent->GetNumberOfSplinePoints();
		MaxDist = SplineComponent->GetDistanceAlongSplineAtSplineInputKey(NumPoints - 1);

		const FTransform SplineTransform = SplineComponent->GetTransformAtDistanceAlongSpline(CurrentDist, ESplineCoordinateSpace::World);
		SetActorLocation(SplineTransform.GetLocation());
		Mesh->SetWorldRotation(SplineTransform.GetRotation());
	}
}

// Called every frame
void ASplineFollowingActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bFollowing)
		FollowSpline(Speed * DeltaTime);
}

// Called to bind functionality to input
void ASplineFollowingActor::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Follow", IE_Pressed, this, &ASplineFollowingActor::StartFollowing);
	PlayerInputComponent->BindAction("Follow", IE_Released, this, &ASplineFollowingActor::StopFollowing);
}

void ASplineFollowingActor::FollowSpline(float Dist)
{
	if (!IsValid(SplineComponent))
		return;

	CurrentDist += Dist;
	if (CurrentDist > MaxDist)
		CurrentDist = 0.0f;
	const FTransform SplineTransform = SplineComponent->GetTransformAtDistanceAlongSpline(CurrentDist, ESplineCoordinateSpace::World);
	SetActorLocation(SplineTransform.GetLocation());
	Mesh->SetWorldRotation(SplineTransform.GetRotation());
}

void ASplineFollowingActor::StartFollowing()
{
	bFollowing = true;
}

void ASplineFollowingActor::StopFollowing()
{
	bFollowing = false;
}

