// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../GameInfo.h"
#include "Components/TileView.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"
#include "TradeWidget.generated.h"

/**
 * 
 */
UCLASS()
class LECTUREGAME_API UTradeWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	// 내가 올리는 거래아이템
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UTileView* PlayerView;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UTileView* PlayerCell;

	// 상대방이 올리는 거래아이템
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UTileView* OtherPlayerView;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UTileView* OtherPlayerCell;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UButton* ExchangeButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UImage* OtherUserTradeCheck;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UButton* CloseButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UTextBlock* PlayerName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UTextBlock* OtherPlayerName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UItemInfoWidget* m_pItemInfo;

	// 거래용 아이템 정보
	UItemData* m_TradeItemData;

	int32 OtherPlayerCharacterNumber;

	bool PlayerReady;
	bool OtherPlayerReady;

	bool m_bMouseHovered;

public:
	virtual void NativeConstruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);
	
public:
	void SetPlayerName(FString Name)
	{
		PlayerName->SetText(FText::FromString(Name));
	}
	void SetOtherPlayerName(FString Name)
	{
		OtherPlayerName->SetText(FText::FromString(Name));
	}
	void SetOtherPlayerCharacterNumber(int32 CharacterNumber)
	{
		OtherPlayerCharacterNumber = CharacterNumber;
	}
	void SetOtherPlayerReady(bool Ready)
	{
		OtherPlayerReady = Ready;
	}

	void ReadyClear()
	{
		OtherPlayerReady = false;
		PlayerReady = false;

		FLinearColor LColor;
		FSlateColor SColor;

		LColor = FLinearColor(0.8f, 0.8f, 0.8f, 1.f);
		ExchangeButton->SetBackgroundColor(LColor);

		SColor = FSlateColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
		OtherUserTradeCheck->SetBrushTintColor(SColor);
	}

	bool GetPlayerReady() const
	{
		return PlayerReady;
	}

	bool GetOtherUserReady() const
	{
		return OtherPlayerReady;
	}

	UFUNCTION()
		void Cancel();
	UFUNCTION()
		void Exchange();
	UFUNCTION()
		void ItemDoubleClick(UObject* pObj);
	UFUNCTION()
		void Item_Hovered(UObject* pObj, bool Hovered);

	void Close();
	void TradeSuccess();
	void AddItem(class UItemData* Item);
	void AddItem(const FString& ItemName, EItemGrade Grade, ItemType Type, int32 Price, int32 Sell, const TArray<FItemOption>& OptionArray);
	void DeleteItem(class UItemData* Item);
	void DeleteItem(int32 Index);

	void TradeReady();

public:
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
};
