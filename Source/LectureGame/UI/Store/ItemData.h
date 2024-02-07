// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../GameInfo.h"
#include "UObject/NoExportTypes.h"
#include "ItemData.generated.h"

USTRUCT(BlueprintType)
struct FItemOptionData
{
	GENERATED_BODY()

public:
	ItemOptionType	Type;
	int32		Option;
};


UCLASS(Blueprintable, BlueprintType)
class LECTUREGAME_API UItemData : public UObject
{
	GENERATED_BODY()

protected:
	FString		m_strName;
	EItemGrade	m_Grade;
	ItemType	m_ItemType;
	int32		m_Price;
	int32		m_Sell;
	TArray<FItemOptionData>	m_ItemOptionArray;

	UTexture2D* m_IconTexture;
	bool	IsEmpty;

	int32	m_ItemIndex;

public:
	int32 GetItemIndex()	const
	{
		return m_ItemIndex;
	}
	bool GetIsEmpty()	const
	{
		return IsEmpty;
	}

	const char* GetNameData()	const
	{
		return TCHAR_TO_ANSI(*m_strName);
	}

	FString GetItemName()	const
	{
		return m_strName;
	}

	EItemGrade GetItemGrade()	const
	{
		return m_Grade;
	}
	ItemType GetItemType()	const
	{
		return m_ItemType;
	}
	int32 GetPrice()	const
	{
		return m_Price;
	}
	int32 GetSell()	const
	{
		return m_Sell;
	}
	UTexture2D* GetIconTexture()	const
	{
		return m_IconTexture;
	}

	int32 GetOptionCount()	const
	{
		return m_ItemOptionArray.Num();
	}

	const FItemOptionData& GetOption(int idx)	const
	{
		return m_ItemOptionArray[idx];
	}

	void SetItemIndex(int32 Index)
	{
		m_ItemIndex = Index;
	}

	void SetIsEmpty(bool Empty)
	{
		IsEmpty = Empty;
	}

	void SetItemName(const FString& pName)
	{
		m_strName = pName;
	}
	void SetItemGrade(EItemGrade Grade)
	{
		m_Grade = Grade;
	}
	void SetItemType(ItemType Type)
	{
		m_ItemType = Type;
	}
	void SetPrice(int32 iPrice)
	{
		m_Price = iPrice;
	}
	void SetSell(int32 iSell)
	{
		m_Sell = iSell;
	}

	void SetIconTexture(const FString& Path)
	{
		m_IconTexture = LoadObject<UTexture2D>(nullptr, *Path);
	}

	void SetIconTexture(UTexture2D* Texture)
	{
		m_IconTexture = Texture;
	}

public:
	void AddOption(ItemOptionType Type, int Option)
	{
		FItemOptionData	Data;

		Data.Type = Type;
		Data.Option = Option;

		m_ItemOptionArray.Add(Data);
	}

	void ClearOption()
	{
		m_ItemOptionArray.Empty();
	}

	void Clear()
	{
		m_strName = nullptr;
		m_IconTexture = nullptr;
		m_ItemType = ItemType::IT_END;
		m_Price = -1;
		m_Sell = -1; 

		IsEmpty = true;
	}
};
