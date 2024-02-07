// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../GameInfo.h"
#include "Components/TileView.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "../ItemInfoWidget.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

UCLASS()
class LECTUREGAME_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UTileView* m_pTileView;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UTileView* m_pTileCell;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UButton* m_pCloseButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UTextBlock* m_pMoneyText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UItemInfoWidget* m_pItemInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UTextBlock* m_ItemCountText;

	UTexture2D* InventoryItemBackGround;

	bool		m_InventoryItemExist;
	bool		m_bOpen;
	bool		m_bOpenTrade;

	bool		m_bMouseHovered;
	bool		m_bMousePressed;
	bool		m_bMousePressStart;
	FVector2D	PressStartPos;

	bool		m_bMouseDrag;

	// 현재 아이템 갯수.
	int32		m_InventoryItemCount;
	int32		m_InventoryItemCountMax;

	// 거래용 아이템 정보
	UItemData* m_UIItemData;

	// 드래그아이템 정보.
	UTexture2D*	m_DragItemIconImage;
	UItemData*	m_DragItemData;

public:
	bool IsExist()	const
	{
		return m_InventoryItemExist;
	}
	void SetExist()
	{
		m_InventoryItemExist = true;
	}
	bool IsOpen()	const
	{
		return m_InventoryItemExist;
	}
	bool IsDrag()	const
	{
		return m_bMouseDrag;
	}
	void SetPress()
	{
		m_bMousePressed = true;
		m_bMousePressStart = true;
	}
	void SetRelease()
	{
		m_bMousePressed = false;
		m_bMouseDrag = false;
	}
	void SetSelectItemIconImage(UTexture2D* icon)
	{
		m_DragItemIconImage = icon;
	}
	void SetSelectItemData(UItemData* data)
	{
		m_DragItemData = data;
	}
	UTexture2D* GetSelectItemIconImage()
	{
		return m_DragItemIconImage;
	}
	UItemData* GetSelectItemData()
	{
		return m_DragItemData;
	}
	int32 GetInventoryItemCount()	const
	{
		return m_pTileView->GetNumItems();
	}
	bool IsFull()	const
	{
		return INVENTORY_MAX == m_InventoryItemCount;
	}

	void SetOpen(bool Open)
	{
		m_bOpen = Open;
	}

	void SetGold(int32 iMoney)
	{
		FString Str = m_pMoneyText->GetText().ToString();
		int32 iGold = FCString::Atoi(*Str);

		if (iGold == 0)
		{
			FString Money = FString::Printf(TEXT("%d"), iMoney);
			m_pMoneyText->SetText(FText::FromString(Money));
		}
		else if (iGold > 0)
		{
			if ((iGold + iMoney) >= 0)
			{
				FString Money = FString::Printf(TEXT("%d"), iGold + iMoney);
				m_pMoneyText->SetText(FText::FromString(Money));
			}
			else
			{
				FString Money = FString::Printf(TEXT("%d"), 0);
				m_pMoneyText->SetText(FText::FromString(Money));
			}
		}

		//FString Money = FString::Printf(TEXT("%d"), iMoney);
		//m_pMoneyText->SetText(FText::FromString(Money));
	}

	void SetInventoryCountMax(int Count)
	{
		m_InventoryItemCountMax = Count;
	}

	void SetInventoryCount(int Count)
	{
		m_InventoryItemCount = Count;

		FString ItemCount = FString::Printf(TEXT("%d  / %d"), m_InventoryItemCount, m_InventoryItemCountMax);
		m_ItemCountText->SetText(FText::FromString(ItemCount));
	}

	void SetTrade(bool Trade)
	{
		m_bOpenTrade = Trade;
	}

	int32 GetInventoryItemCount()
	{
		return m_InventoryItemCount;
	}

	void Clear()
	{
		m_DragItemIconImage = nullptr;
		m_DragItemData = nullptr;
	}
public:
	virtual void NativeConstruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

public:
	UFUNCTION()
	void ItemDoubleClick(UObject* pObj);

	UFUNCTION()
	void Item_Hovered(UObject* pObj, bool Hovered);

	UFUNCTION()
	void CloseInventory();

public:
	void AddItem(const FString& ItemName, EItemGrade Grade, ItemType Type, int32 Price, int32 Sell, const TArray<FItemOption>& OptionArray);
	void AddItem(class UItemData* Data);
	void DeleteItem(class UItemData* Data);
	bool IndexMoveItem(int32 Source, int32 Dest);

public:
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);

	/*virtual FReply NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent);
	virtual void NativeOnFocusLost(const FFocusEvent& InFocusEvent);
	virtual void NativeOnFocusChanging(const FWeakWidgetPath& PreviousFocusPath, const FWidgetPath& NewWidgetPath, const FFocusEvent& InFocusEvent);
	virtual void NativeOnAddedToFocusPath(const FFocusEvent& InFocusEvent);
	virtual void NativeOnRemovedFromFocusPath(const FFocusEvent& InFocusEvent);
	virtual FNavigationReply NativeOnNavigation(const FGeometry& MyGeometry, const FNavigationEvent& InNavigationEvent, const FNavigationReply& InDefaultReply);
	virtual FReply NativeOnKeyChar(const FGeometry& InGeometry, const FCharacterEvent& InCharEvent);
	virtual FReply NativeOnPreviewKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent);
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent);
	virtual FReply NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent);
	virtual FReply NativeOnAnalogValueChanged(const FGeometry& InGeometry, const FAnalogInputEvent& InAnalogEvent);
	virtual FReply NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent);
	virtual FReply NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual FReply NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation);
	virtual void NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation);
	virtual void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation);
	virtual bool NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation);
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation);
	virtual void NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation);
	virtual FReply NativeOnTouchGesture(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent);
	virtual FReply NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent);
	virtual FReply NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent);
	virtual FReply NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent);
	virtual FReply NativeOnMotionDetected(const FGeometry& InGeometry, const FMotionEvent& InMotionEvent);
	virtual FReply NativeOnTouchForceChanged(const FGeometry& MyGeometry, const FPointerEvent& TouchEvent);*/
};
