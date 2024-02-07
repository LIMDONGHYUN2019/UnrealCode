// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../GameInfo.h"
#include "Components/ListView.h"
#include "Components/Button.h"
#include "Blueprint/UserWidget.h"
#include "StoreWidget.generated.h"


UCLASS()
class LECTUREGAME_API UStoreWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UListView* ItemList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UButton* CloseButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UItemInfoWidget* m_pItemInfo;

	bool		m_bMouseHovered;

	bool		FirstOpen;
	bool		bOpen;

public:
	bool IsFirstOpen()	const
	{
		return FirstOpen;
	}
	void IsFirstClose()
	{
		FirstOpen = true;	
	}
	void Open()
	{
		bOpen = true;
	}
	void Close()
	{
		bOpen = false;
	}
	bool IsOpen()	 const
	{
		return bOpen;
	}

public:
	virtual void NativeConstruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime); // update

public:
	UFUNCTION()
	void ItemDoubleClick(UObject* pObj);

	UFUNCTION()
	void ItemClick(UObject* pObj);

	UFUNCTION()
	void CloseStore();

	UFUNCTION()
		void Item_Hovered(UObject* pObj, bool Hovered);

public:
	void AddItem(const FString& ItemName, EItemGrade Grade, ItemType Type, int32 Price, int32 Sell, const TArray<FItemOption>& OptionArray);
};
