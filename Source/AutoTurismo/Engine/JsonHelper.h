// Copyright Nat1Studios All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "JsonObjectConverter.h"
#include "JsonHelper.generated.h"

/**
 * JSON helper utility.
 */
UCLASS()
class AUTOTURISMO_API UJsonHelper : public UObject
{
	GENERATED_BODY()

public:
	/**
	 * Save struct to JSON file on disk.
	 *
	 * @param SaveStruct : Struct to save.
	 * @param Path : JSON file path.
	 * @return True if save successful, false otherwise.
	 */
	template<typename InStructType>
	static bool SaveStructToJson(const InStructType& SaveStruct, const FString& Path)
	{
		FString JsonString = "";
		if (FJsonObjectConverter::UStructToJsonObjectString<InStructType>(SaveStruct, JsonString))
		{
			return FFileHelper::SaveStringToFile(JsonString, *Path);
		}
		return false;
	}

	/**
	 * Load struct from JSON file on disk.
	 *
	 * @param[out] LoadStruct : Struct to load.
	 * @param Path : JSON file path.
	 * @return True if load successful, false otherwise.
	 */
	template<typename OutStructType>
	static bool LoadStructFromJson(OutStructType& LoadStruct, const FString& Path)
	{
		if (FPaths::FileExists(Path))
		{
			FString JsonString;
			FFileHelper::LoadFileToString(JsonString, *Path);
			return FJsonObjectConverter::JsonObjectStringToUStruct<OutStructType>(JsonString, &LoadStruct);
		}
		return false;
	}
};
