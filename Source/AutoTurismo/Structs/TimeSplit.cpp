// Copyright Nat1Studios All Rights Reserved.

#include "TimeSplit.h"

FString FLapSplit::ToString() const
{
	static FNumberFormattingOptions TimeFormatOption = FNumberFormattingOptions::DefaultNoGrouping();
	TimeFormatOption.SetMaximumFractionalDigits(2);
	TimeFormatOption.SetMinimumFractionalDigits(2);

	FString OutString = "Lap: ";
	for (int i = 0; i < Splits.Num(); i++)
	{
		OutString += FText::AsNumber(Splits[i], &TimeFormatOption).ToString();
		if (i != Splits.Num() - 1)
			OutString += ", ";
	}
	return OutString;
}

float FRaceSplit::AddSplit(int LapNum, float Time)
{
	if (!Laps.IsValidIndex(LapNum))
		return 0.0f;

	const float PreviousSplitTime = Laps[LapNum].Splits.Num() > 0 ? Laps[LapNum].Splits.Last() : (LapNum > 0 ? Laps[LapNum - 1].Splits.Last() : 0.0f);
	Laps[LapNum].Splits.Add(Time);
	return Time - PreviousSplitTime;
}

float FRaceSplit::GetLapTime(int LapNum) const
{
	if (!Laps.IsValidIndex(LapNum) || Laps[LapNum].Splits.Num() == 0)
		return 0.0f;

	return Laps[LapNum].Splits.Last() - (LapNum > 0 ? Laps[LapNum - 1].Splits.Last() : 0.0f);
}

FString FRaceSplit::ToString() const
{
	FString OutString = "Race: \n";
	for (int i = 0; i < Laps.Num(); i++)
	{
		OutString += Laps[i].ToString();
		if (i != Laps.Num() - 1)
			OutString += "\n";
	}
	return OutString;
}

