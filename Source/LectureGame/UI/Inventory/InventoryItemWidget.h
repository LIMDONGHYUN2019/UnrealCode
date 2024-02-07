#pragma once

#include "../../GameInfo.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Blueprint/UserWidget.h"
#include "InventoryItemWidget.generated.h"

enum class DragItemType : UINT8
{
	InventoryItem,
	SlotItem,
	TradeItem,
	EnchanceItem,
	ThrowItem
};

UCLASS()
class LECTUREGAME_API UInventoryItemWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UButton* m_ItemIcon;
	// �� button�̶� border�� ���� �־�� �ϴ°���? �ϳ��� �־ ������� �ʳ�?
	// InventoryWidget���� Ŭ����������Ʈ�� �ָ��ؼ� ���⼭ Button�� �������.

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UBorder* m_ItemImage;

	class UItemData*  m_Data;
	class UTexture2D* m_Image;

	DragItemType	DragType;

	class UInventoryWidget* m_pInventory;

public:
	virtual void NativeConstruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime); // update
	                                                                                                                                                                                                                        
public:
	UFUNCTION(BlueprintCallable)
	void SetData(class UItemData* pData);

	void SetDragItemType(DragItemType Data)
	{
		DragType = Data;
	}
	DragItemType GetDragItemType()
	{
		return DragType;
	}

	void SetIconImage(UTexture2D* Texture)
	{
		m_ItemImage->SetBrushFromTexture(Texture);
		m_Image = Texture;
	}

	void SetItemData(UItemData* Data)
	{
		m_Data = Data;
	}

	UTexture2D* GetItemIconImage()
	{
		return m_Image;
	}

	UItemData* GetItemData()
	{
		return m_Data;
	}

	void Clear(UTexture2D* InventoryItemBackGround)
	{
		m_ItemImage->SetBrushFromTexture(InventoryItemBackGround);
		m_Image = InventoryItemBackGround;
	}

public:
	UFUNCTION()
	void ButtonDown();

	UFUNCTION()
	void ButtonUp();
};
