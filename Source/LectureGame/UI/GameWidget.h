// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Skill/SkillIconWidget.h"
#include "Blueprint/UserWidget.h"
#include "GameWidget.generated.h"

UCLASS()
class LECTUREGAME_API UGameWidget : public UUserWidget
{
	GENERATED_BODY()
	

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UChattingWidget* ChatWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UStoreWidget* StoreWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class USkillWidget* SkillWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UInventoryWidget* InventoryWidget;
	
	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UPlayerStatusWidget* PlayerStatusWidget;

	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UProgressBar* ExpBar;

	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UMinimapWidget* Minimap;

	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UTradeWidget* TradeWidget;

	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UPlayerInfoWidget* PlayerInfoWidget;

	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UTradeRequestWidget* TradeRequestWidget;

	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UGroupWidget* GroupWidget;

	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UGroupRequestWidget* GroupRequestWidget;

	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class USevarogRoomCreateWidget* SevarogRoomWidget;

	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UMainPortalWidget* MainPortalWidget;

	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class USlotListWidget* SlotListWidget;

	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class USlotListItemWidget* SlotListItemWidget;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class USkillIconWidget>	DragWidgetClass;
	class USkillIconWidget* DragWidget; // 스킬용

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UInventoryItemWidget>	ItemDragWidgetClass;
	class UInventoryItemWidget* ItemDragWidget; // 아이템용

	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UQuestWidget* QuestWidget;

	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UInformationWidget* InformationWidget;

	UCanvasPanel* GameCanvas;
	UCanvasPanelSlot* DragWidgetSlot;

	FVector2D		m_MousePos;

	bool			ComputeSize;

public:
	class USevarogRoomCreateWidget* GetSevarogRoomCreateWidget()
	{
		return SevarogRoomWidget;
	}

	class UMainPortalWidget* GetMainPortalWidget()
	{
		return MainPortalWidget;
	}

	class UPlayerInfoWidget* GetPlayerInfoWidget()
	{
		return PlayerInfoWidget;
	}

	class UStoreWidget* GetStoreWidget()
	{
		return StoreWidget;
	}

	class USkillWidget* GetSkillWidget()
	{
		return SkillWidget;
	}

	class UInventoryWidget* GetInventoryWidget()
	{
		return InventoryWidget;
	}
	
	class UPlayerStatusWidget* GetPlayerStatusWidget()
	{
		return PlayerStatusWidget;
	}

	class UChattingWidget* GetChatWidget()
	{
		return ChatWidget;
	}

	class UMinimapWidget* GetMinimapWidget()
	{
		return Minimap;
	}

	class UTradeWidget* GetTradeWidget()
	{
		return TradeWidget;
	}

	class UTradeRequestWidget* GetTradeRequestWidget()
	{
		return TradeRequestWidget;
	}

	class UGroupWidget* GetGroupWidget()
	{
		return GroupWidget;
	}

	class UGroupRequestWidget* GetGroupRequestWidget()
	{
		return GroupRequestWidget;
	}

	class USlotListWidget* GetSlotWidget()
	{
		return SlotListWidget;
	}

	class USlotListItemWidget* GetSlotItemWidget()
	{
		return SlotListItemWidget;
	}

	class UQuestWidget* GetQuestWidget()
	{
		return QuestWidget;
	}

	class UInformationWidget* GetInformationWidget()
	{
		return InformationWidget;
	}

public:
	virtual void NativeOnInitialized();
	virtual void NativeConstruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime); // update
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);

public:
	void OpenStoreWidget();
	void OpenSkillWidget();
	void OpenQuestWidget();
	void OpenInventoryWidget();
	void OpenTradeWidget();
	void OpenPlayerInfoWidget(class AUserCharacter* pPlayer);
	void OpenTradeRequestWidget();
	void OpenGroupWidget();
	void OpenGroupRequestWidget();
	void OpenMainPortalWidget();


	void ClosePlayerInfoWidget();
	void SetExpPercent(float Percent);
};
