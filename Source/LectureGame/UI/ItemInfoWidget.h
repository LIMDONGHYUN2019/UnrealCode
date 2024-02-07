// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Store/ItemData.h"
#include "Components/Button.h"
#include "ItemInfoWidget.generated.h"

/**
 * 
 */
UCLASS()
class LECTUREGAME_API UItemInfoWidget : public UUserWidget
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UTextBlock* m_Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UTextBlock* m_Grade;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UTextBlock* m_Type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UTextBlock* m_Buy;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UTextBlock* m_Sell;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UTextBlock* m_Index;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UTextBlock*	m_Option;

public:
	virtual void NativeConstruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime); // update

public:
	void SetItemName(const FString& Name)
	{
		m_Name->SetText(FText::FromString(Name));
	}

	void SetItemGrade(EItemGrade Data)
	{
		FSlateColor color;

		switch (Data)
		{
		case EItemGrade::Normal:
			{ // 하얀색
				m_Grade->SetText(FText::FromString(TEXT("일반 등급")));
				color = FSlateColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
				m_Name->SetColorAndOpacity(color);
				m_Grade->SetColorAndOpacity(color);
			}
			break;
		case EItemGrade::Rare:
			{ // 파란색
				m_Grade->SetText(FText::FromString(TEXT("레어 등급")));
				color = FSlateColor(FLinearColor(0.086624f, 0.322002f, 1.0f, 1.f));
				m_Name->SetColorAndOpacity(color);
				m_Grade->SetColorAndOpacity(color);
			}
			break;
		case EItemGrade::Epic:
			{ // 노랑색
				m_Grade->SetText(FText::FromString(TEXT("에픽 등급")));
				color = FSlateColor(FLinearColor(0.286458f, 0.225038f, 0.117523f, 1.f));
				m_Name->SetColorAndOpacity(color);
				m_Grade->SetColorAndOpacity(color);
			}
			break;
		case EItemGrade::Unique:
			{// 빨강색
				m_Grade->SetText(FText::FromString(TEXT("유니크 등급")));
				color = FSlateColor(FLinearColor(1.f, 0.056774f, 0.086621f, 1.f));
				m_Name->SetColorAndOpacity(color);
				m_Grade->SetColorAndOpacity(color);
			}
			break;
		case EItemGrade::Legendary:
			{ // 주황색
				m_Grade->SetText(FText::FromString(TEXT("전설 등급")));
				color = FSlateColor(FLinearColor(1.f, 0.464488f, 0.031026f, 1.f));
				m_Name->SetColorAndOpacity(color);
				m_Grade->SetColorAndOpacity(color);
			}
			break;
		default:
			break;
		}
	}

	void SetItemBuy(const int32& Buy)
	{
		FString  Tempo = FString::Printf(TEXT("%d $$"), Buy);
		m_Buy->SetText(FText::FromString(Tempo));
	}

	void SetItemSell(const int32& Sell)
	{
		FString  Tempo = FString::Printf(TEXT("%d $$"), Sell);
		m_Sell->SetText(FText::FromString(Tempo));
	}

	void SetItemOptionType(ItemType Type)
	{
		switch (Type)
		{
		case ItemType::IT_WEAPON:
			m_Type->SetText(FText::FromString(TEXT("무기")));
			break;
		case ItemType::IT_ARMOR:
			m_Type->SetText(FText::FromString(TEXT("갑옷")));
			break;
		case ItemType::IT_HELMET:
			m_Type->SetText(FText::FromString(TEXT("헬멧")));
			break;
		case ItemType::IT_SHIELD:
			m_Type->SetText(FText::FromString(TEXT("방패")));
			break;
		case ItemType::IT_POTION:
			m_Type->SetText(FText::FromString(TEXT("포션")));
			break;
		}
	}

	void SetOption(ItemOptionType  Option, int32 Data)
	{
		FString Str = m_Option->GetText().ToString();
		FString OptionData;

		switch (Option)
		{
		case IOT_ATTACK:
			OptionData = FString::Printf(TEXT("%d"), Data);
			m_Option->SetText(FText::FromString(Str + TEXT("공격력  ") + OptionData + TEXT("  증가") + TEXT("\n")));
			break;
		case IOT_ARMOR:
			OptionData = FString::Printf(TEXT("%d"), Data);
			m_Option->SetText(FText::FromString(Str + TEXT("방어력  ") + OptionData + TEXT("  증가") + TEXT("\n")));
			break;
		case IOT_HP:
			OptionData = FString::Printf(TEXT("%d"), Data);
			m_Option->SetText(FText::FromString(Str + TEXT("최대 HP  ") + OptionData + TEXT("  증가") + TEXT("\n")));
			break;
		case IOT_MP:
			OptionData = FString::Printf(TEXT("%d"), Data);
			m_Option->SetText(FText::FromString(Str + TEXT("최대 MP  ") + OptionData + TEXT("  증가") + TEXT("\n")));
			break;
		case IOT_HPRECOVERY:
			OptionData = FString::Printf(TEXT("%d"), Data);
			m_Option->SetText(FText::FromString(Str + TEXT("HP  ") + OptionData + TEXT("  회복") + TEXT("\n")));
			break;
		case IOT_MPRECOVERY:
			OptionData = FString::Printf(TEXT("%d"), Data);
			m_Option->SetText(FText::FromString(Str + TEXT("MP  ") + OptionData + TEXT("  회복") + TEXT("\n")));
			break;
		case IOT_CRITICALRATIO:
			OptionData = FString::Printf(TEXT("%d"), Data);
			m_Option->SetText(FText::FromString(Str + TEXT("Critical 확률  ") + OptionData + TEXT("  증가") + TEXT("\n")));
			break;
		case IOT_CRITICALDAMAGE:
			OptionData = FString::Printf(TEXT("%d"), Data);
			m_Option->SetText(FText::FromString(Str + TEXT("Critical Damage  ") + OptionData + TEXT("  증가") + TEXT("\n")));
			break;
		case IOT_MOVESPEED:
			OptionData = FString::Printf(TEXT("%d"), Data);
			m_Option->SetText(FText::FromString(Str + TEXT("이속  ") + OptionData + TEXT("  증가") + TEXT("\n")));
			break;
		case IOT_END:
			break;
		}
	}

	void OptionClear()
	{
		m_Option->SetText(FText::FromString(TEXT("")));
	}

	void SetIndex(int32 Index)
	{
		FString  Tempo = FString::Printf(TEXT("%d"), Index);
		m_Index->SetText(FText::FromString(Tempo));
	}
	
	void SetItemInfo(UItemData* Info)
	{
		SetItemName(Info->GetItemName());
		SetItemGrade(Info->GetItemGrade());
		SetItemBuy(Info->GetPrice());
		SetItemSell(Info->GetSell());
		SetItemOptionType(Info->GetItemType());
		SetIndex(Info->GetItemIndex());

		OptionClear();

		int32 iOptionCount = Info->GetOptionCount();

		for (int i = 0; i < Info->GetOptionCount(); ++i)
		{
			const FItemOptionData ItemOption = Info->GetOption(i);
			SetOption(ItemOption.Type, ItemOption.Option);
		}
	}

};
