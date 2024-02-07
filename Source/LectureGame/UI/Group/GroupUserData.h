#pragma once

#include "../../GameInfo.h"
#include "UObject/NoExportTypes.h"
#include "GroupUserData.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class LECTUREGAME_API UGroupUserData : public UObject
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		FString Job;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		FString Level;

public:
	void SetName(FString& pName)
	{
		Name = pName;
	}
	void SetJob(FString& pJob)
	{
		Job = pJob;
	}
	void SetLevel(int32 iLevel)
	{
		 Level = FString::Printf(TEXT("%d"), iLevel);
	}
	// ==================
	FString GetName()
	{
		return Name;
	}
	FString GetJob()
	{
		return Job;
	}
	FString GetLevel()
	{
		return Level;
	}
	
};
