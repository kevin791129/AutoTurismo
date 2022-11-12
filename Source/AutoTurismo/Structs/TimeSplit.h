// Copyright Nat1Studios All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TimeSplit.generated.h"

/**
 * Lap split times.
 */
USTRUCT(BlueprintType, meta = (DisplayName = "Lap Split"))
struct AUTOTURISMO_API FLapSplit
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Time Split")
		TArray<float> Splits;

	FLapSplit() { }
	FLapSplit(int NumSplits) { Splits.Reserve(NumSplits); }

	/**
	 * Lap split as string.
	 * 
	 * @return : String representation of lap split.
	 */
	FString ToString() const;
};

/**
 * Race split times.
 */
USTRUCT(BlueprintType, meta = (DisplayName = "Race Split"))
struct AUTOTURISMO_API FRaceSplit
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Time Split")
		TArray<FLapSplit> Laps;

	FRaceSplit() { }
	FRaceSplit(int NumLaps, int SplitsPerLap) { for (int i = 0; i < NumLaps; i++) Laps.Add(FLapSplit(SplitsPerLap)); }

	/**
	 * Add time split.
	 *
	 * @param LapNum : Lap index.
	 * @param Time : Split time.
	 * @return : Time elapsed since previous split.
	 */
	float AddSplit(int LapNum, float Time);

	/**
	 * Get lap time.
	 *
	 * @param LapNum : Lap index.
	 * @return : Time taken for specific lap.
	 */
	float GetLapTime(int LapNum) const;

	/**
	 * Race split as string.
	 *
	 * @return : String representation of race split.
	 */
	FString ToString() const;
};
