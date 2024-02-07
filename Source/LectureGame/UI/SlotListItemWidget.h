// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/Border.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Store/ItemData.h"
#include "ItemInfoWidget.h"
#include "SlotListItemWidget.generated.h"

/**
 *
 */
UCLASS()
class LECTUREGAME_API USlotListItemWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TArray<UButton*>	SlotArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TArray<UBorder*>	SlotIconArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UItemInfoWidget*	ItemInfoWidget;

	TArray<UTexture2D*>	m_ItemIconImageArray;
	//TArray<FItemInfoData*>	ItemInfoArray;
	TArray<UItemData*>	ItemDataArray;
	// UItemData Ŭ������ �ұ� �ƴϸ� FItemInfoData ����ü�� �ұ�
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
	bool IsDrag()	const
	{
		return m_bMouseDrag;
	}

	void Check(UItemData* Data, int32 HitIndex)
	{
		// 1. ���������� �������� 2. ����Ÿ������ 3. �̸��� ���� ���Կ� �ִ��� �񱳸� �ؾ��Ѵ�.
		// 2�� ���ִ»��¿��� a, b
		// b��ġ���� �κ��丮���� a�� ��������� ����2�� ��ġ�� �ٲ��.
		// b��ġ���� b�� ��������ϱ� ������ �����ϳ��� b�� �Ǿ������.
		// ���� result�� 2�����µ� Compare�� �������� �ٸ��� 2���Ǿ ���� ���ϴ°� ����.
		int32 Index = -1;

		if (Data->GetItemType() != ItemType::IT_POTION)
			return;

		for (auto result : ItemDataArray)
		{
			Index++;
			if (!result)
				continue;
			if (Data->GetItemType() == ItemType::IT_POTION)
			{
				if (HitIndex == -1 && result->GetItemName().Compare(Data->GetItemName()) == 0)
				{
					SlotIconArray[Index]->SetBrushFromTexture(nullptr);
					ItemDataArray[Index] = nullptr;

					return;
				}

				if (result->GetItemName().Compare(Data->GetItemName()) == 0)// ������
				{
					if (Index != HitIndex)
					{
						if (ItemDataArray[HitIndex])
						{
							UTexture2D* pTempo = m_ItemIconImageArray[HitIndex];
							SlotIconArray[Index]->SetBrushFromTexture(pTempo);

							m_ItemIconImageArray[Index] = pTempo;
							ItemDataArray[Index] = ItemDataArray[HitIndex];
						}
						else
						{
							SlotIconArray[Index]->SetBrushFromTexture(nullptr);
							ItemDataArray[Index] = nullptr;
						}
					}

					return;
				}
			}
		}
	}

	void ComputeWorldPos(const FVector2D& Pos, const FVector2D& Ratio);
	int32 CollisionSlot(const FVector2D& MousePos);

	void SetIconImage(UTexture2D* Texture, int32 HitIndex)
	{
		SlotIconArray[HitIndex]->SetBrushFromTexture(Texture);
		m_ItemIconImageArray[HitIndex] = Texture;
	}

	void SetItemData(UItemData* Data, int32 HitIndex)
	{
		ItemDataArray[HitIndex] = Data;
	}

	UTexture2D* GetSlotIconImage()	const
	{
		if (m_DragSlotIndex == -1)
			return nullptr;

		return m_ItemIconImageArray[m_DragSlotIndex];
	}

	UItemData* GetSlotItemInfo()
	{
		if (m_DragSlotIndex == -1)
			return nullptr;

		return ItemDataArray[m_DragSlotIndex];
	}
	UItemData* GetSlotItemInfo(int Index)
	{
		if (Index == -1)
			return nullptr;

		return ItemDataArray[Index];
	}

public:
	UFUNCTION()
		void Item1_Hovered();
	UFUNCTION()
		void Item2_Hovered();

	UFUNCTION()
		void Item1_UnHovered();
	UFUNCTION()
		void Item2_UnHovered();

	UFUNCTION()
		void Item1_Pressed();
	UFUNCTION()
		void Item2_Pressed();

	UFUNCTION()
		void Item1_Released();
	UFUNCTION()
		void Item2_Released();
};
