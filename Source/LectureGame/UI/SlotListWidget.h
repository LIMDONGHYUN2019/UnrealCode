// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/Border.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Skill/SkillInfoWidget.h"
#include "SlotListWidget.generated.h"

UCLASS()
class LECTUREGAME_API USlotListWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<UButton*>	SlotArray;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<UBorder*>	SlotIconArray;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USkillInfoWidget*	SkillInfoWidget;

	TArray<UTexture2D*>	m_SkillIconImageArray;
	TArray<SkillInfo*>	SkillInfoArray;

	TArray<ESlotType>	SkillTypeArray;
	TArray<Rectangle>	SlotRectArray;

	bool		m_bMouseHovered;
	bool		m_bMousePressed;
	bool		m_bMousePressStart;
	bool		m_bMouseDrag;

	int32		m_DragSlotIndex;

	FVector2D	PressStartPos;

public:
	virtual void NativeConstruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime); // update

public:
	void Check(int32 SkillNumber, int32 HitIndex)
	{
		// 슬롯에 이미 이 스킬이 있는지 체크
		// 이미 있다면 기존에 있던거를 지워준다.
		int32 Index = -1;

		for (auto result : SkillInfoArray)
		{
			Index++;

			if (!result)
				continue;

			if (result->iSkillNumber == SkillNumber)
			{
				if (HitIndex != -1 && SkillInfoArray[HitIndex])
				{
					UTexture2D* pTempo = m_SkillIconImageArray[HitIndex];
					SlotIconArray[Index]->SetBrushFromTexture(pTempo);

					m_SkillIconImageArray[Index] = pTempo;
					SkillInfoArray[Index] = SkillInfoArray[HitIndex];
				}

				else
				{
					SlotIconArray[Index]->SetBrushFromTexture(nullptr);
					SkillInfoArray[Index] = nullptr;
				}
			}
		}
	}

	bool IsDrag()	const
	{
		return m_bMouseDrag;
	}

	void ComputeWorldPos(const FVector2D& Pos, const FVector2D& Ratio);
	int32 CollisionSlot(const FVector2D& MousePos);

	void SetIconImage(UTexture2D* Texture, int32 HitIndex)
	{
		SlotIconArray[HitIndex]->SetBrushFromTexture(Texture);
		m_SkillIconImageArray[HitIndex] = Texture;
	}

	void SetSkillInfo(SkillInfo* Info, int32 SkillLevel, int32 HitIndex)
	{
		SkillInfoArray[HitIndex] = Info;
	}

	UTexture2D* GetSlotIconImage()	const
	{
		if (m_DragSlotIndex == -1)
			return nullptr;

		return m_SkillIconImageArray[m_DragSlotIndex];
	}

	SkillInfo* GetSlotSkillInfo()
	{
		if (m_DragSlotIndex == -1)
			return nullptr;

		return SkillInfoArray[m_DragSlotIndex];
	}
	SkillInfo* GetSlotSkillInfo(int Index)
	{
		if (Index == -1)
			return nullptr;

		return SkillInfoArray[Index];
	}

public:
	UFUNCTION()
		void Slot1Hovered();

	UFUNCTION()
		void Slot2Hovered();

	UFUNCTION()
		void Slot3Hovered();

	UFUNCTION()
		void Slot4Hovered();
	//--------------------------
	UFUNCTION()
		void Slot1UnHovered();

	UFUNCTION()
		void Slot2UnHovered();

	UFUNCTION()
		void Slot3UnHovered();

	UFUNCTION()
		void Slot4UnHovered();
	//--------------------------
	UFUNCTION()
		void Slot1Pressed();

	UFUNCTION()
		void Slot2Pressed();

	UFUNCTION()
		void Slot3Pressed();

	UFUNCTION()
		void Slot4Pressed();
	//--------------------------
	UFUNCTION()
		void Slot1Released();

	UFUNCTION()
		void Slot2Released();

	UFUNCTION()
		void Slot3Released();

	UFUNCTION()
		void Slot4Released();
};
