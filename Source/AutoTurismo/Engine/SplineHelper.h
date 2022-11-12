// Copyright Nat1Studios All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SplineHelper.generated.h"

class USplineComponent;

/**
 * Spline segment type.
 */
UENUM(BlueprintType, meta = (DisplayName = "Spline Segment Type"))
enum class ESplineSegmentType : uint8
{
	Straight	UMETA(DisplayName = "Straight"),
	Right		UMETA(DisplayName = "Right Turn"),
	Left		UMETA(DisplayName = "Left Turn"),
	Invalid		UMETA(Hidden)
};

/**
 * Spline segment information.
 */
USTRUCT(BlueprintType, meta = (DisplayName = "Spline Segment Info"))
struct FSplineSegmentInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Spline")
		ESplineSegmentType Type;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Spline")
		int StartKey;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Spline")
		int EndKey;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Spline")
		float Length;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Spline")
		float DeltaAngle;

	FSplineSegmentInfo() : Type(ESplineSegmentType::Invalid), StartKey(0), EndKey(0), Length(0.0f), DeltaAngle(0.0f) { }

	FORCEINLINE bool IsValid() const { return Type != ESplineSegmentType::Invalid; }

	friend class USplineHelper;

private:
	/**
	 * Set segment information.
	 * 
	 * @param InStartKey : Start key value.
	 * @param InEndKey : End key value.
	 * @param InLength : Length value.
	 * @param InDeltaAngle : Delta angle value.
	 */
	void SetKeys(int InStartKey, int InEndKey, float InLength, float InDeltaAngle);
};

/**
 * Spline helper utility.
 */
UCLASS()
class AUTOTURISMO_API USplineHelper : public UObject
{
	GENERATED_BODY()

public:
	/**
	 * Blueprint helper to check validity of spline segment info.
	 * 
	 * @param Info : Spline segment info.
	 * @return True if info is valid, false otherwise.
	 */
	UFUNCTION(BlueprintPure, Category = Spline, meta = (DisplayName = "Get Spline Info Valid"))
		static bool BP_CheckSplineSegmentValid(const FSplineSegmentInfo& Info);

	/**
	 * Get info on spline segment beginning at start key and ending at start key + 1.
	 *
	 * @param Spline : Spline component.
	 * @param bClosedLoop : True if spline is a closed loop, false otherwise.
	 * @param StartKey : Segment start.
	 * @return Spline segment information.
	 */
	UFUNCTION(BlueprintPure, Category = Spline)
		static FSplineSegmentInfo GetSplineSegmentInfo(const USplineComponent* Spline, bool bClosedLoop, int StartKey);

	/**
	 * Get info on all spline segments.
	 *
	 * @param Spline : Spline component.
	 * @param bClosedLoop : True if spline is a closed loop, false otherwise.
	 * @param[out] OutInfo : List of segment info.
	 * @return : True if all info is valid, false otherwise.
	 */
	UFUNCTION(BlueprintPure, Category = Spline)
		static bool GetSplineAllSegmentInfo(const USplineComponent* Spline, bool bClosedLoop, UPARAM(ref) TArray<FSplineSegmentInfo>& OutInfo);
};
