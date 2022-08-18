// Copyright Nat1Studios All Rights Reserved.


#include "AutoTurismoUtility.h"
#include "AutoTurismo.h"

void UAutoTurismoUtility::Log(const FString& Msg)
{
	UE_LOG(LogAutoTurismo, Log, TEXT("%s"), *Msg)
}