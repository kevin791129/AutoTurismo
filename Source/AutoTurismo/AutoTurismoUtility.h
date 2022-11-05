// Copyright Nat1Studios All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AutoTurismoUtility.generated.h"

/**
 * Console log types.
 */
UENUM(BlueprintType, meta = (DisplayName = "Log Verbosity"))
enum class ELogType : uint8
{
	Log		UMETA(DisplayName = "Log"),
	Warning UMETA(DisplayName = "Warning"),
	Error	UMETA(DisplayName = "Error`")
};

/**
 * Blueprint utility functions.
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
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Console Log"))
		static void Log(const FString& Msg, ELogType Verbosity = ELogType::Log);

	/**
	 * Get whether game is currently running in editor.
	 *
	 * @return True if running in editor, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
		static bool GetInEditor();
};
