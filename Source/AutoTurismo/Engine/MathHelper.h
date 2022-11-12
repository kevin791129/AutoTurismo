// Copyright Nat1Studios All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MathHelper.generated.h"

/**
 * Math helper utility.
 */
UCLASS()
class AUTOTURISMO_API UMathHelper : public UObject
{
	GENERATED_BODY()

public:
	/**
	 * Get CW or CCW angle between from and to vectors (normalized).
	 *
	 * @param From : Normalized from vector.
	 * @param To : Normalized to vector.
	 * @param Normal : Projection plane normal to determine CW or CCW rotation.
	 * @return : Positive angle for CW, negative angle for CCW in radians.
	 */
	UFUNCTION(BlueprintPure, Category = Math)
		static float GetAngleBetweenVectors(const FVector& From, const FVector& To, const FVector& Normal);
};
