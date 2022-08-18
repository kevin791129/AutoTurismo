// Copyright Nat1Studios All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AutoTurismoUtility.generated.h"

/**
 * 
 */
UCLASS()
class AUTOTURISMO_API UAutoTurismoUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	/**
	 * Blueprint log to console helper function.
	 *
	 * @param Msg : Message to be logged.
	 */
	UFUNCTION(BlueprintCallable)
		static void Log(const FString& Msg);
};
