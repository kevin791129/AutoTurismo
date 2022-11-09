// Copyright Nat1Studios All Rights Reserved.


#include "MathHelper.h"

float UMathHelper::GetAngleBetweenVectors(const FVector& From, const FVector& To, const FVector& Normal)
{
	const float AngleSign = FVector::DotProduct(FVector::CrossProduct(From, To), Normal) >= 0.0f ? 1.0f : -1.0f;
	return FMath::Acos(FVector::DotProduct(From, To)) * AngleSign;
}