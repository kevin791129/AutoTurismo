// Copyright Nat1Studios All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DriveSettings.generated.h"

/**
 * Drive settings.
 */
USTRUCT(BlueprintType, meta = (DisplayName = "Drive Settings"))
struct AUTOTURISMO_API FDriveSettings
{
	GENERATED_BODY()

	/* Brake input when throttle released */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Drive Settings")
		float BrakeInput = 1.0f;
	/* Position ahead to peak */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Drive Settings|Steering")
		float PeakDist = 0.2f;
	/* Distance away from spline squared for angle lerp */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Drive Settings|Steering")
		float DistanceThreshold = 40000.0f;
	/* Multiplier to forward angle */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Drive Settings|Steering")
		float ForwardMultiplier = 1.0f;
	/* Multiplier to position angle */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Drive Settings|Steering")
		float PositionMultiplier = 1.0f;
	/* Multiplier to smooth steering */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Drive Settings|Steering")
		float SteeringMultiplier = 0.1f;
};
