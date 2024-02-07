// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../GameInfo.h"
#include "UObject/NoExportTypes.h"
#include "QuestListData.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class LECTUREGAME_API UQuestListData : public UObject
{
	GENERATED_BODY()
	
public:
	UQuestListData();

private:
	_tagQuestInfo		m_Info;

public:
	const _tagQuestInfo& GetQuestInfo()	const
	{
		return m_Info;
	}

public:
	void SetQuestInfo(const _tagQuestInfo& Info)
	{
		m_Info = Info;
	}
};
