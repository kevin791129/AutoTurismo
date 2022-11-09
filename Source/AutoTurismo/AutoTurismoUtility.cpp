// Copyright Nat1Studios All Rights Reserved.


#include "AutoTurismoUtility.h"
#include "Engine/JsonHelper.h"
#include "AutoTurismo.h"

void UAutoTurismoUtility::Log(const FString& Msg, ELogType Verbosity)
{
	switch (Verbosity)
	{
	case ELogType::Log:
	default:
		{
			UE_LOG(LogAutoTurismo, Log, TEXT("%s"), *Msg)
			break;
		}
	case ELogType::Warning:
		{
			UE_LOG(LogAutoTurismo, Warning, TEXT("%s"), *Msg)
			break;
		}
	case ELogType::Error:
		{
			UE_LOG(LogAutoTurismo, Error, TEXT("%s"), *Msg)
			break;
		}
	}
}

bool UAutoTurismoUtility::GetInEditor()
{
#if WITH_EDITOR
	return true;
#else
	return false;
#endif
}

bool UAutoTurismoUtility::TryParseFloat_String(const FString& Text, UPARAM(ref) float& OutNumber)
{
	if (!FCString::IsNumeric(*Text))
		return false;

	OutNumber = FCString::Atof(*Text);
	return true;
}

bool UAutoTurismoUtility::TryParseFloat_Text(const FText& Text, UPARAM(ref) float& OutNumber)
{
	return TryParseFloat_String(Text.ToString(), OutNumber);
}

bool UAutoTurismoUtility::SaveDriveSettings(const FDriveSettings& InSettings, const FString& Path)
{
	return UJsonHelper::SaveStructToJson(InSettings, Path);
}

bool UAutoTurismoUtility::LoadDriveSettings(UPARAM(ref) FDriveSettings& OutSettings, const FString& Path)
{
	return UJsonHelper::LoadStructFromJson(OutSettings, Path);
}