// Copyright Nat1Studios All Rights Reserved.


#include "AutoTurismoUtility.h"
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