// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "UObject/NoExportTypes.h"
#include "ChatData.generated.h"

UCLASS(Blueprintable, BlueprintType)
class LECTUREGAME_API UChatData : public UObject
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FString	m_ChatText;

	bool m_bNotice;

public:
	void SetText(const FString& Text)
	{
		m_ChatText = Text;
	}

	void SetNotice(bool Notice)
	{
		m_bNotice = Notice;
	}

	FString GetText()	const
	{
		return m_ChatText;
	}
	bool GetNotice()	const
	{
		return m_bNotice;
	}
};
