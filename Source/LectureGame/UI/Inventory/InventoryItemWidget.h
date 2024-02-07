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
	// 왜 button이랑 border가 같이 있어야 하는거지? 하나만 있어도 상관없지 않나?
	// InventoryWidget에서 클릭델리게이트가 애매해서 여기서 Button을 만들었다.

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
