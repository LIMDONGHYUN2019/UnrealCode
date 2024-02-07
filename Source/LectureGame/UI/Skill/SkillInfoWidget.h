// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../GameInfo.h"
#include "Components/ListView.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"
#include "SkillInfoWidget.generated.h"

UCLASS()
class LECTUREGAME_API USkillInfoWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UTextBlock* m_Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UTextBlock* m_Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UTextBlock* m_Property;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UTextBlock* m_SkillLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UTextBlock* m_SkillLevelMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UTextBlock* m_RequiredLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UTextBlock* m_Cooldown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UTextBlock*	m_Option;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UTextBlock* m_Desc;

public:
	virtual void NativeConstruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime); // update

public:
	void SetSkillInfo(const SkillInfo* Info)
	{
		FString Name = UTF8_TO_TCHAR(Info->strName);
		FString Desc = UTF8_TO_TCHAR(Info->strDesc);

		m_Name->SetText(FText::FromString(Name));
		m_Desc->SetText(FText::FromString(Desc));

		int32 iCooldown = Info->fCooldown;
		FString  Cooldown = FString::Printf(TEXT("%d"), iCooldown);
		m_Cooldown->SetText(FText::FromString(TEXT("쿨타임  ") + Cooldown + TEXT("초")));

		FString  LevelMax = FString::Printf(TEXT("%d"), Info->iSkillLevelMax);
		m_SkillLevelMax->SetText(FText::FromString(LevelMax));

		FString RequiredLevel = FString::Printf(TEXT("%d"), Info->iRequiredLevel);
		m_RequiredLevel->SetText(FText::FromString(TEXT("제한 레벨 : ") + RequiredLevel));

		SetSkillType(Info->eType);
		SetSkillProperty(Info->ePropt);

		OptionClear();
		for (int32 iCount = 0; iCount < Info->vecOption.Num(); ++iCount)
		{
			SetSkillOption(Info->vecOption[iCount].SkillOptionType, Info->vecOption[iCount].SkillOptionData);
		}

		//현재 레벨 추가해야함.
	}

	void OptionClear()
	{
		m_Option->SetText(FText::FromString(TEXT("")));
	}

	void SetSkillName(const FString& Name)
	{
		m_Name->SetText(FText::FromString(Name));
	}

	void SetSkillDesc(const FString& Desc)
	{
		m_Desc->SetText(FText::FromString(Desc));
	}

	void SetSkillCooldown(const int& Cooldown)
	{
		FString  Tempo= FString::Printf(TEXT("%d 초"), Cooldown);
		m_Cooldown->SetText(FText::FromString(Tempo));
	}

	void SetSkillLevel(const FString& Level)
	{
		m_SkillLevel->SetText(FText::FromString(Level));
	}

	void SetSkillLevelMax(const int& LevelMax)
	{
		FString  Tempo = FString::Printf(TEXT("%d"), LevelMax);
		m_SkillLevelMax->SetText(FText::FromString(Tempo));
	}

	void SetSkillRequiredLevel(const int& RequiredLevel)
	{
		FString  Tempo = FString::Printf(TEXT("%d"), RequiredLevel);
		m_RequiredLevel->SetText(FText::FromString(Tempo));
	}

	void SetSkillType(ESkillType Type)
	{
		FSlateColor color;

		switch (Type)
		{
		case ESkillType::Passive:
			m_Type->SetText(FText::FromString(TEXT("[ 패시브 ]")));
			color = FSlateColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
			m_Type->SetColorAndOpacity(color);
			break;
		case ESkillType::Active:
			m_Type->SetText(FText::FromString(TEXT("[ 액티브 ]")));
			color = FSlateColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
			m_Type->SetColorAndOpacity(color);
			break;
		}
	}

	void SetSkillProperty(ESkillProperty Property)
	{
		FSlateColor color;

		switch (Property)
		{
		case ESkillProperty::Physical:
			m_Property->SetText(FText::FromString(TEXT("[ 물리타입 ]")));
			color = FSlateColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
			m_Property->SetColorAndOpacity(color);
			break;
		case ESkillProperty::Magical:
			m_Property->SetText(FText::FromString(TEXT("[ 마법타입 ]")));
			color = FSlateColor(FLinearColor(0.79252f, 0.029669f, 1.f, 1.f));
			break;
		case ESkillProperty::Fire:
			m_Property->SetText(FText::FromString(TEXT("[ 불 속성 ]")));
			color = FSlateColor(FLinearColor(1.f, 0.027538f, 0.f, 1.f));
			m_Property->SetColorAndOpacity(color);
			break;
		case ESkillProperty::Frozen:
			m_Property->SetText(FText::FromString(TEXT("[ 얼음 속성 ]")));
			color = FSlateColor(FLinearColor(0.f, 0.209389f, 1.f, 1.f));
			m_Property->SetColorAndOpacity(color);
			break;
		case ESkillProperty::Lightning:
			m_Property->SetText(FText::FromString(TEXT("[ 전기 속성 ]")));
			color = FSlateColor(FLinearColor(0.909127f, 1.f, 0.f, 1.f));
			m_Property->SetColorAndOpacity(color);
			break;
		case ESkillProperty::Posion:
			m_Property->SetText(FText::FromString(TEXT("[ 독 속성 ]")));
			color = FSlateColor(FLinearColor(0.193394f, 1.f, 0.092479f, 1.f));
			m_Property->SetColorAndOpacity(color);
			break;
		case ESkillProperty::Divine:
			m_Property->SetText(FText::FromString(TEXT("[ 성 속성 ]")));
			color = FSlateColor(FLinearColor(1.f, 0.763733f, 0.176878f, 1.f));
			m_Property->SetColorAndOpacity(color);
			break;
		}
	}

	void SetSkillOption(ESkillOptionType Option, float Data)
	{
		FString Str = m_Option->GetText().ToString();
		FString OptionData;
		int32 iData = Data;

		switch (Option)
		{
		case ESkillOptionType::Damage:
			OptionData = FString::Printf(TEXT("%d"), iData);
			m_Option->SetText(FText::FromString(Str + TEXT("데미지  ") + OptionData + TEXT(" 피해") + TEXT("\n")));
			break;
		case ESkillOptionType::PercentDamage:
			OptionData = FString::Printf(TEXT("%.1f"), Data);
			m_Option->SetText(FText::FromString(Str + TEXT("데미지  ") + OptionData + TEXT(" 피해") + TEXT("\n")));
			break;
		case ESkillOptionType::AugmentPropt:
			OptionData = FString::Printf(TEXT("%d"), iData);
			m_Option->SetText(FText::FromString(Str + OptionData + TEXT("초 동안 속성을 부여합니다.") + TEXT("\n")));
			break;
		case ESkillOptionType::ATKUp:
			OptionData = FString::Printf(TEXT("%d"), iData);
			m_Option->SetText(FText::FromString(Str + TEXT("공격력  ") + OptionData + TEXT(" 증가") + TEXT("\n")));
			break;
		case ESkillOptionType::DEFUp:
			OptionData = FString::Printf(TEXT("%d"), iData);
			m_Option->SetText(FText::FromString(Str + TEXT("방어력  ") + OptionData + TEXT(" 증가") + TEXT("\n")));
			break;
		case ESkillOptionType::ATKDown:
			OptionData = FString::Printf(TEXT("%d"), iData);
			m_Option->SetText(FText::FromString(Str + TEXT("공격력  ") + OptionData + TEXT(" 감소") + TEXT("\n")));
			break;
		case ESkillOptionType::DEFDown:
			OptionData = FString::Printf(TEXT("%d"), iData);
			m_Option->SetText(FText::FromString(Str + TEXT("방어력  ") + OptionData + TEXT(" 감소") + TEXT("\n")));
			break;
		case ESkillOptionType::Heals:
			OptionData = FString::Printf(TEXT("%d"), iData);
			m_Option->SetText(FText::FromString(Str + TEXT("체력  ") + OptionData + TEXT(" 회복") + TEXT("\n")));
			break;
		case ESkillOptionType::PercentHeals:
			OptionData = FString::Printf(TEXT("%d"), iData);
			m_Option->SetText(FText::FromString(Str + TEXT("체력  ") + OptionData + TEXT("% 회복") + TEXT("\n")));
			break;
		case ESkillOptionType::EnhancesHP:
			OptionData = FString::Printf(TEXT("%d"), iData);
			m_Option->SetText(FText::FromString(Str + TEXT("최대 체력  ") + OptionData + TEXT(" 증가") + TEXT("\n")));
			break;
		case ESkillOptionType::EnhancesMP:
			OptionData = FString::Printf(TEXT("%d"), iData);
			m_Option->SetText(FText::FromString(Str + TEXT("최대 마나  ") + OptionData + TEXT(" 회복") + TEXT("\n")));
			break;
		case ESkillOptionType::HPDown:
			break;
		case ESkillOptionType::HPRecovery:
			OptionData = FString::Printf(TEXT("%d"), iData);
			m_Option->SetText(FText::FromString(Str + TEXT("체력 회복속도  ") + OptionData + TEXT(" 증가") + TEXT("\n")));
			break;
		case ESkillOptionType::MPRecovery:
			OptionData = FString::Printf(TEXT("%d"), iData);
			m_Option->SetText(FText::FromString(Str + TEXT("마나 회복속도  ") + OptionData + TEXT(" 증가") + TEXT("\n")));
			break;
		case ESkillOptionType::IncreaseMoveSpeed:
			OptionData = FString::Printf(TEXT("%d"), iData);
			m_Option->SetText(FText::FromString(Str + TEXT("이동속도  ") + OptionData + TEXT(" 증가") + TEXT("\n")));
			break;
		case ESkillOptionType::IncreaseATKSpeed:
			OptionData = FString::Printf(TEXT("%d"), iData);
			m_Option->SetText(FText::FromString(Str + TEXT("공격속도  ") + OptionData + TEXT("% 증가") + TEXT("\n")));
			break;
		case ESkillOptionType::SlowDown:
			OptionData = FString::Printf(TEXT("%d"), iData);
			m_Option->SetText(FText::FromString(Str + TEXT("이동속도  ") + OptionData + TEXT(" 감소") + TEXT("\n")));
			break;
		case ESkillOptionType::Cripple:
			OptionData = FString::Printf(TEXT("%d"), iData);
			m_Option->SetText(FText::FromString(Str + OptionData + TEXT("초 동안 상태이상으로 만듭니다.") + TEXT("\n")));
			break;
		case ESkillOptionType::DamageReturned:
			OptionData = FString::Printf(TEXT("%d"), iData);
			m_Option->SetText(FText::FromString(Str + TEXT("피격시 데미지 ") + OptionData + TEXT(" 반사") + TEXT("\n")));
			break;
		case ESkillOptionType::DamageImmunity:
			OptionData = FString::Printf(TEXT("%d"), iData);
			m_Option->SetText(FText::FromString(Str + TEXT("데미지 면역  ") + OptionData + TEXT(" 초") + TEXT("\n")));
			break;
		case ESkillOptionType::PercentLifeSteal:
			OptionData = FString::Printf(TEXT("%d"), iData);
			m_Option->SetText(FText::FromString(Str + TEXT("공격시 체력 ") + OptionData + TEXT("% 흡수") + TEXT("\n")));
			break;
		case ESkillOptionType::Bleed:
			OptionData = FString::Printf(TEXT("%d"), iData);
			m_Option->SetText(FText::FromString(Str + OptionData + TEXT("초 동안 출혈상태으로 만듭니다.") + TEXT("\n")));
			break;
		case ESkillOptionType::Poison:
			OptionData = FString::Printf(TEXT("%d"), iData);
			m_Option->SetText(FText::FromString(Str + OptionData + TEXT("초 동안 중독상태으로 만듭니다.") + TEXT("\n")));
			break;
		case ESkillOptionType::Burn:
			OptionData = FString::Printf(TEXT("%d"), iData);
			m_Option->SetText(FText::FromString(Str + OptionData + TEXT("초 동안 화상상태으로 만듭니다.") + TEXT("\n")));
			break;
		case ESkillOptionType::Shock:
			OptionData = FString::Printf(TEXT("%d"), iData);
			m_Option->SetText(FText::FromString(Str + OptionData + TEXT("초 동안 감전상태으로 만듭니다.") + TEXT("\n")));
			break;
		case ESkillOptionType::Freeze:
			OptionData = FString::Printf(TEXT("%d"), iData);
			m_Option->SetText(FText::FromString(Str + OptionData + TEXT("초 동안 빙결상태으로 만듭니다.") + TEXT("\n")));
			break;
		case ESkillOptionType::Stun:
			OptionData = FString::Printf(TEXT("%d"), iData);
			m_Option->SetText(FText::FromString(Str + OptionData + TEXT("초 동안 스턴상태으로 만듭니다.") + TEXT("\n")));
			break;
		case ESkillOptionType::BlindEnemy:
			OptionData = FString::Printf(TEXT("%d"), iData);
			m_Option->SetText(FText::FromString(Str + OptionData + TEXT("초 동안 실명상태으로 만듭니다.") + TEXT("\n")));
			break;
		case ESkillOptionType::Weaken:
			OptionData = FString::Printf(TEXT("%d"), iData);
			m_Option->SetText(FText::FromString(Str + OptionData + TEXT("초 동안 약화으로 만듭니다.") + TEXT("\n") + TEXT("약화 : 능력치 절반으로 감소")));
			break;
		case ESkillOptionType::Summon:
			OptionData = FString::Printf(TEXT("%d"), iData);
			m_Option->SetText(FText::FromString(Str + OptionData + TEXT("초 동안 소환수을 소환합니다.") + TEXT("\n")));
			break;
		case ESkillOptionType::UnSummon:
			m_Option->SetText(FText::FromString(Str + TEXT("소환수을 해제합니다.") + TEXT("\n")));
			break;
		case ESkillOptionType::Sneak:
			OptionData = FString::Printf(TEXT("%d"), iData);
			m_Option->SetText(FText::FromString(Str + OptionData + TEXT("초 동안 은신상태으로 만듭니다.") + TEXT("\n")));
			break;
		case ESkillOptionType::Taunt:
			OptionData = FString::Printf(TEXT("%d"), iData);
			m_Option->SetText(FText::FromString(Str + OptionData + TEXT("초 동안 도발") + TEXT("\n")));
			break;
		case ESkillOptionType::Kinematic:
			OptionData = FString::Printf(TEXT("%d"), iData);
			m_Option->SetText(FText::FromString(Str + TEXT("블랙홀을 만들어 ") + OptionData + TEXT("초 동안 끌어당기게 합니다.") + TEXT("\n")));
			break;
		case ESkillOptionType::Transform:
			OptionData = FString::Printf(TEXT("%d"), iData);
			m_Option->SetText(FText::FromString(Str + OptionData + TEXT("초 동안 변신을 합니다.") + TEXT("\n")));
			break;
		case ESkillOptionType::Polymorph:
			break;
		case ESkillOptionType::Resurrection:
			break;
		}
	}
};
