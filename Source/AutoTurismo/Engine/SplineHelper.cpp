// Copyright Nat1Studios All Rights Reserved.

#include "SplineHelper.h"
#include "Components/SplineComponent.h"
#include "MathHelper.h"

#define STRAIGHT_THRESHOLD 0.5235988f

void FSplineSegmentInfo::SetKeys(int InStartKey, int InEndKey, float InLength, float InDeltaAngle)
{
	StartKey = InStartKey;
	EndKey = InEndKey;
	Length = InLength;
	DeltaAngle = InDeltaAngle;
	if (DeltaAngle > STRAIGHT_THRESHOLD)
		Type = ESplineSegmentType::Right;
	else if (DeltaAngle < -STRAIGHT_THRESHOLD)
		Type = ESplineSegmentType::Left;
	else
		Type = ESplineSegmentType::Straight;
}

bool USplineHelper::BP_CheckSplineSegmentValid(const FSplineSegmentInfo& Info)
{
	return Info.IsValid();
}

FSplineSegmentInfo USplineHelper::GetSplineSegmentInfo(const USplineComponent* Spline, bool bClosedLoop, int StartKey)
{
	FSplineSegmentInfo Result;
	if (IsValid(Spline))
	{
		const int NumKeys = Spline->GetNumberOfSplinePoints();
		if (bClosedLoop ? NumKeys > StartKey : NumKeys - 1 > StartKey)
		{
			// Get segment end key
			int EndKey = StartKey + 1;
			if (bClosedLoop)
				EndKey %= NumKeys;

			const FVector StartDir = Spline->GetDirectionAtSplineInputKey(StartKey, ESplineCoordinateSpace::Local);
			const FVector EndDir = Spline->GetDirectionAtSplineInputKey(EndKey, ESplineCoordinateSpace::Local);
			const float StartDist = Spline->GetDistanceAlongSplineAtSplineInputKey(StartKey);
			const float EndDist = Spline->GetDistanceAlongSplineAtSplineInputKey(EndKey);

			Result.SetKeys(StartKey, EndKey, EndDist - StartDist, UMathHelper::GetAngleBetweenVectors(StartDir, EndDir, FVector::UpVector));
		}
	}
	return Result;

}

bool USplineHelper::GetSplineAllSegmentInfo(const USplineComponent* Spline, bool bClosedLoop, UPARAM(ref) TArray<FSplineSegmentInfo>& OutInfo)
{
	OutInfo.Reset();
	if (!IsValid(Spline))
		return false;

	const int NumKeys = Spline->GetNumberOfSplinePoints();
	const int ValidLastKey = NumKeys - (bClosedLoop ? 1 : 2);
	for (int i = 0; i <= ValidLastKey; i++)
	{
		FSplineSegmentInfo TempInfo = GetSplineSegmentInfo(Spline, bClosedLoop, i);
		if (!TempInfo.IsValid())
			return false;
		OutInfo.Emplace(TempInfo);
	}
	return true;
}