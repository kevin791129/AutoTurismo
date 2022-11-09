// Copyright Nat1Studios All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Structs/DriveSettings.h"
#include "AutoTurismoUtility.generated.h"

/**
 * Console log types.
 */
UENUM(BlueprintType, meta = (DisplayName = "Log Verbosity"))
enum class ELogType : uint8
{
	Log		UMETA(DisplayName = "Log"),
	Warning UMETA(DisplayName = "Warning"),
	Error	UMETA(DisplayName = "Error")
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

	/**
	 * Try to parse floating point number from text.
	 *
	 * @param Text : Number as text.
	 * @param[out] OutNumber : Parsed number.
	 * @return True if parse successful, false otherwise.
	 */
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Try Parse Float (String)"))
		static bool TryParseFloat_String(const FString& Text, UPARAM(ref) float& OutNumber);

	/**
	 * Try to parse floating point number from text.
	 * 
	 * @param Text : Number as text.
	 * @param[out] OutNumber : Parsed number.
	 * @return True if parse successful, false otherwise.
	 */
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Try Parse Float (Text)"))
		static bool TryParseFloat_Text(const FText& Text, UPARAM(ref) float& OutNumber);

#pragma region Struct IO
	/**
	 * Blueprint wrapper for saving drive settings to JSON.
	 * 
	 * @see Engine/JsonHelper.h
	 */
	UFUNCTION(BlueprintPure)
		static bool SaveDriveSettings(const FDriveSettings& InSettings, const FString& Path);

	/**
	 * Blueprint wrapper for loading drive settings from JSON.
	 *
	 * @see Engine/JsonHelper.h
	 */
	UFUNCTION(BlueprintPure)
		static bool LoadDriveSettings(UPARAM(ref) FDriveSettings& OutSettings, const FString& Path);
#pragma endregion
};
