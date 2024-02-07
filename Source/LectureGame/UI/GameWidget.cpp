// Fill out your copyright notice in the Description page of Project Settings.


#include "GameWidget.h"
#include "../Chatting/ChattingWidget.h"
#include "Store/StoreWidget.h"
#include "Skill/SkillWidget.h"
#include "Inventory/InventoryWidget.h"
#include "Inventory/InventoryItemWidget.h"
#include "MinimapWidget.h"
#include "PlayerUI/PlayerStatusWidget.h"
#include "PlayerUI/TradeWidget.h"
#include "PlayerUI/TradeRequestWidget.h"
#include "PlayerUI/PlayerInfoWidget.h"
#include "Group/GroupWidget.h"
#include "Group/GroupRequestWidget.h"
#include "../OtherUser/UserCharacter.h"
#include "SevarogRoomCreateWidget.h"
#include "MainPortalWidget.h"
#include "Components/ProgressBar.h"
#include "SlotListWidget.h"
#include "SlotListItemWidget.h"
#include "Quest/QuestWidget.h"
#include "InformationWidget.h"

void UGameWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	LOG(TEXT("Canvas Size : %s"), *GetDesiredSize().ToString());
}

void UGameWidget::NativeConstruct()
{
	Super::NativeConstruct();
	ChatWidget = Cast<UChattingWidget>(GetWidgetFromName(TEXT("UI_ChattingWindow")));
	StoreWidget = Cast<UStoreWidget>(GetWidgetFromName(TEXT("UI_store")));
	InventoryWidget = Cast<UInventoryWidget>(GetWidgetFromName(TEXT("UI_Inventory")));
	SkillWidget = Cast<USkillWidget>(GetWidgetFromName(TEXT("UI_Skill")));
	PlayerStatusWidget = Cast<UPlayerStatusWidget>(GetWidgetFromName(TEXT("UI_PlayerStatus")));
	ExpBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("UI_Exp")));
	Minimap = Cast<UMinimapWidget>(GetWidgetFromName(TEXT("UI_Minimap")));
	TradeWidget = Cast<UTradeWidget>(GetWidgetFromName(TEXT("UI_Trade")));
	PlayerInfoWidget = Cast<UPlayerInfoWidget>(GetWidgetFromName(TEXT("UI_PlayerInfo")));
	TradeRequestWidget = Cast<UTradeRequestWidget>(GetWidgetFromName(TEXT("UI_TradeRequest")));
	GroupWidget = Cast<UGroupWidget>(GetWidgetFromName(TEXT("UI_Group")));
	GroupRequestWidget = Cast<UGroupRequestWidget>(GetWidgetFromName(TEXT("UI_GroupRequest")));
	SevarogRoomWidget = Cast<USevarogRoomCreateWidget>(GetWidgetFromName(TEXT("UI_SevarogRoomNPC")));
	MainPortalWidget = Cast<UMainPortalWidget>(GetWidgetFromName(TEXT("UI_MainPortal")));

	MainPortalWidget = Cast<UMainPortalWidget>(GetWidgetFromName(TEXT("UI_MainPortal")));

	SlotListWidget = Cast<USlotListWidget>(WidgetTree->FindWidget(TEXT("UI_SlotList")));
	SlotListItemWidget = Cast<USlotListItemWidget>(WidgetTree->FindWidget(TEXT("UI_SlotList_Item")));

	QuestWidget = Cast<UQuestWidget>(GetWidgetFromName(TEXT("UI_Quest")));

	InformationWidget = Cast<UInformationWidget>(GetWidgetFromName(TEXT("UI_Information")));



	UCanvasPanelSlot*	CurrentSlot = Cast<UCanvasPanelSlot>(Slot);

	LOG(TEXT("Canvas Desired Size : %s"), *GetDesiredSize().ToString());

	GameCanvas = Cast<UCanvasPanel>(WidgetTree->FindWidget(TEXT("CanvasPanel_0")));

	DragWidget = nullptr;
	DragWidgetSlot = nullptr;

	ComputeSize = false;
}

void UGameWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!ComputeSize)
	{
		ComputeSize = true;

		const FGeometry& geometry = GetCachedGeometry();

		FVector2D LocalSize = MyGeometry.GetLocalSize();
		LOG(TEXT("Canvas Geometry Local Size : %s"), *LocalSize.ToString());

		FVector2D	ScreenPos = MyGeometry.LocalToAbsolute(FVector2D(0.f, 0.f));
		LOG(TEXT("Canvas Geometry Screen Pos : %s"), *ScreenPos.ToString());

		FVector2D	ScreenSize = MyGeometry.LocalToAbsolute(LocalSize) - ScreenPos;
		LOG(TEXT("Canvas Geometry Screen Size : %s"), *ScreenSize.ToString());

		{// SkillSlot
			UCanvasPanelSlot* SlotListPanel = Cast<UCanvasPanelSlot>(SlotListWidget->Slot);

			FVector2D Pos = SlotListPanel->GetPosition();
			FVector2D Size = SlotListPanel->GetSize();

			Pos.X += LocalSize.X * 0.5f;
			Pos.Y += LocalSize.Y * 0.5f;

			// ���� ����� ��ġ�� ���Ѵ�.
			FVector2D Alignment = SlotListPanel->GetAlignment();

			Pos.X = Pos.X - Alignment.X * Size.X;
			Pos.Y = Pos.Y - Alignment.Y * Size.Y;

			LOG(TEXT("Pos : %s, Size : %s"), *Pos.ToString(), *Size.ToString());

			FVector2D	Ratio;
			Ratio.X = ScreenSize.X / LocalSize.X;
			Ratio.Y = ScreenSize.Y / LocalSize.Y;

			SlotListWidget->ComputeWorldPos(Pos, Ratio);
		}

		{// ItemSlot
			UCanvasPanelSlot* ItemSlotListPanel = Cast<UCanvasPanelSlot>(SlotListItemWidget->Slot);
			FVector2D Pos = ItemSlotListPanel->GetPosition();
			FVector2D Size = ItemSlotListPanel->GetSize();

			Pos.X += LocalSize.X * 0.5f;
			Pos.Y += LocalSize.Y * 0.5f;

			// ���� ����� ��ġ�� ���Ѵ�.
			FVector2D Alignment = ItemSlotListPanel->GetAlignment();

			Pos.X = Pos.X - Alignment.X * Size.X;
			Pos.Y = Pos.Y - Alignment.Y * Size.Y;

			LOG(TEXT("Pos : %s, Size : %s"), *Pos.ToString(), *Size.ToString());

			FVector2D	Ratio;
			Ratio.X = ScreenSize.X / LocalSize.X;
			Ratio.Y = ScreenSize.Y / LocalSize.Y;

			SlotListItemWidget->ComputeWorldPos(Pos, Ratio);
		}

		{
			// �ŷ��ý��� �巡�׷� ǥ���ҷ��ߴµ� �ϴ� Ŭ������ ��ü
			//UCanvasPanelSlot* TradePanel = Cast<UCanvasPanelSlot>(TradeWidget->Slot);
			//FVector2D Pos = TradePanel->GetPosition();
			//FVector2D Size = TradePanel->GetSize();

			//Pos.X += LocalSize.X * 0.5f;
			//Pos.Y += LocalSize.Y * 0.5f;

			//// ���� ����� ��ġ�� ���Ѵ�.
			//FVector2D Alignment = TradePanel->GetAlignment();

			//Pos.X = Pos.X - Alignment.X * Size.X;
			//Pos.Y = Pos.Y - Alignment.Y * Size.Y;

			//LOG(TEXT("Pos : %s, Size : %s"), *Pos.ToString(), *Size.ToString());

			//FVector2D	Ratio;
			//Ratio.X = ScreenSize.X / LocalSize.X;
			//Ratio.Y = ScreenSize.Y / LocalSize.Y;
		}
	}

	if (SkillWidget->IsDrag())
	{
		// �巡�׸� �����ϴ� ���¶�� �巡���� ��ų ������ ������ �������ֵ��� �Ѵ�.
		// �巡�� ���۽��������� �Ǵ��ؾ� �Ѵ�.
		// ������� �巡���ϴ°��� SkillIcon / Titlebar���� ��������.
		if(SkillWidget->GetSkillIconDrag())
		{
			if (!DragWidget)
			{
				DragWidget = WidgetTree->ConstructWidget<USkillIconWidget>(DragWidgetClass);

				DragWidgetSlot = GameCanvas->AddChildToCanvas(DragWidget);
				DragWidgetSlot->SetZOrder(3);

				SkillInfo* Info = SkillWidget->GetSelectSkillInfo();
				int32		Level = SkillWidget->GetSkillLevel();
				UTexture2D* IconImage = SkillWidget->GetSelectSkillIconImage();

				DragWidget->SetIconImage(IconImage);
				DragWidget->SetSkillInfo(Info, Level);
			}

			else
			{
				// �巡�� ������ ��ġ�� ���콺�� ��ġ�� ���ؼ� �������ش�.
				FVector2D	PixelPosition, ViewportMin, ViewportMax;

				USlateBlueprintLibrary::LocalToViewport(GetWorld(), MyGeometry, FVector2D(0.f, 0.f), PixelPosition, ViewportMin);
				USlateBlueprintLibrary::LocalToViewport(GetWorld(), MyGeometry, MyGeometry.GetLocalSize(), PixelPosition, ViewportMax);

				FVector2D	MousePos = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());

				MousePos -= ViewportMin;

				UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(DragWidget->Slot);

				CanvasSlot->SetPosition(FVector2D(MousePos.X, MousePos.Y));
				CanvasSlot->SetAlignment(FVector2D(0.5f, 0.5f));
				CanvasSlot->SetSize(FVector2D(70.f, 70.f));
			}
		}
		else if (SkillWidget->GetTitlebarDrag())
		{
			FVector2D	PixelPosition, ViewportMin, ViewportMax;

			USlateBlueprintLibrary::LocalToViewport(GetWorld(), MyGeometry, FVector2D(0.f, 0.f), PixelPosition, ViewportMin);
			USlateBlueprintLibrary::LocalToViewport(GetWorld(), MyGeometry, MyGeometry.GetLocalSize(), PixelPosition, ViewportMax);

			FVector2D	MousePos = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());

			MousePos -= ViewportMin;

			FVector2D Test = SkillWidget->GetDistance();

			UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(SkillWidget->Slot);

			FVector2D MouseClick = MousePos - Test;

			CanvasSlot->SetPosition(FVector2D(MouseClick.X, MouseClick.Y));
			FVector2D Size = CanvasSlot->GetSize();
		}
	}

	else if (InventoryWidget->IsDrag())
	{
		if (!ItemDragWidget)
		{
			ItemDragWidget = WidgetTree->ConstructWidget<UInventoryItemWidget>(ItemDragWidgetClass);

			DragWidgetSlot = GameCanvas->AddChildToCanvas(ItemDragWidget);
			DragWidgetSlot->SetZOrder(3);

			UTexture2D* IconImage = InventoryWidget->GetSelectItemIconImage();
			UItemData* ItemData = InventoryWidget->GetSelectItemData();
			ItemDragWidget->SetIconImage(IconImage);
			ItemDragWidget->SetItemData(ItemData);
			ItemDragWidget->SetDragItemType(DragItemType::InventoryItem);

		}
		else
		{
			FVector2D	PixelPosition, ViewportMin, ViewportMax;
			USlateBlueprintLibrary::LocalToViewport(GetWorld(), MyGeometry, FVector2D(0.f, 0.f), PixelPosition, ViewportMin);
			USlateBlueprintLibrary::LocalToViewport(GetWorld(), MyGeometry, MyGeometry.GetLocalSize(), PixelPosition, ViewportMax);

			FVector2D	MousePos = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());

			MousePos -= ViewportMin;

			//PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("x = %.5f y = %.5f"), MousePos.X, MousePos.Y));

			UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(ItemDragWidget->Slot);
			CanvasSlot->SetPosition(FVector2D(MousePos.X, MousePos.Y));
			CanvasSlot->SetAlignment(FVector2D(0.5f, 0.5f));
			CanvasSlot->SetSize(FVector2D(70.f, 70.f));
		}
	}

	else if (SlotListWidget->IsDrag())
	{
		if (!DragWidget)
		{
			DragWidget = WidgetTree->ConstructWidget<USkillIconWidget>(DragWidgetClass);

			DragWidgetSlot = GameCanvas->AddChildToCanvas(DragWidget);
			DragWidgetSlot->SetZOrder(3);

			UTexture2D* IconImage = SlotListWidget->GetSlotIconImage();
			SkillInfo* Info = SlotListWidget->GetSlotSkillInfo();
			int32		Level = SkillWidget->GetSkillLevel();

			DragWidget->SetIconImage(IconImage);
			DragWidget->SetSkillInfo(Info, Level);
		}

		else
		{
			FVector2D	PixelPosition, ViewportMin, ViewportMax;

			USlateBlueprintLibrary::LocalToViewport(GetWorld(), MyGeometry, FVector2D(0.f, 0.f), PixelPosition, ViewportMin);
			USlateBlueprintLibrary::LocalToViewport(GetWorld(), MyGeometry, MyGeometry.GetLocalSize(), PixelPosition, ViewportMax);

			FVector2D	MousePos = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());

			MousePos -= ViewportMin;

			//PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("x = %.5f y = %.5f"), MousePos.X, MousePos.Y));

			UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(DragWidget->Slot);

			CanvasSlot->SetPosition(FVector2D(MousePos.X, MousePos.Y));
			CanvasSlot->SetAlignment(FVector2D(0.5f, 0.5f));
			CanvasSlot->SetSize(FVector2D(70.f, 70.f));
		}
	}

	else if (SlotListItemWidget->IsDrag())
	{
		if (!ItemDragWidget)
		{
			ItemDragWidget = WidgetTree->ConstructWidget<UInventoryItemWidget>(ItemDragWidgetClass);

			DragWidgetSlot = GameCanvas->AddChildToCanvas(ItemDragWidget);
			DragWidgetSlot->SetZOrder(3);

			UTexture2D* IconImage = SlotListItemWidget->GetSlotIconImage();
			UItemData* ItemData = SlotListItemWidget->GetSlotItemInfo();

			ItemDragWidget->SetIconImage(IconImage);
			ItemDragWidget->SetItemData(ItemData);
			ItemDragWidget->SetDragItemType(DragItemType::SlotItem);
		}

		else
		{
			FVector2D	PixelPosition, ViewportMin, ViewportMax;

			USlateBlueprintLibrary::LocalToViewport(GetWorld(), MyGeometry, FVector2D(0.f, 0.f), PixelPosition, ViewportMin);
			USlateBlueprintLibrary::LocalToViewport(GetWorld(), MyGeometry, MyGeometry.GetLocalSize(), PixelPosition, ViewportMax);

			FVector2D	MousePos = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());

			MousePos -= ViewportMin;

			UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(ItemDragWidget->Slot);

			CanvasSlot->SetPosition(FVector2D(MousePos.X, MousePos.Y));
			CanvasSlot->SetAlignment(FVector2D(0.5f, 0.5f));
			CanvasSlot->SetSize(FVector2D(70.f, 70.f));
		}
	}

	// ���⼭ ���� �巡�װ� �ߴܵɋ�
	else if (DragWidget)
	{
		// �巡�� �ϰ� �ִ� ���콺�� ���� ���콺�� ��ġ�� ���Ͽ� ���� �ȿ� ������ �Ǵ��Ѵ�.
		FVector2D	PixelPosition, ViewportMin, ViewportMax;

		USlateBlueprintLibrary::LocalToViewport(GetWorld(), MyGeometry, FVector2D(0.f, 0.f), PixelPosition, ViewportMin);
		USlateBlueprintLibrary::LocalToViewport(GetWorld(), MyGeometry, MyGeometry.GetLocalSize(), PixelPosition, ViewportMax);

		FVector2D	MousePos = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());

		MousePos -= ViewportMin;

		// ���� ����Ʈ�� �簢�� ������ �浹ó���� �����Ѵ�.

		// ���Ը���Ʈ �ȿ� �簢�� ������ �����Ƿ� �� ���콺 ��ġ�� �̿��Ͽ� �ش� �簢����� �浹ó���� �غ��� �浹�Ǵ°� �ִٸ�
		// �ش� �簢�� �ȿ� �������� �־��־�� �Ѵ�.

		// ���Ը���Ʈ���� ���� �� �������� �巡�װ� �ǰ��ִ�.
		int32	HitIndex = SlotListWidget->CollisionSlot(MousePos);

		if (HitIndex != -1)
		{
			LOG(TEXT("Slot Index : %d"), HitIndex);
			// ���⼭ ��ųâ���� �����°����� ���� ���Կ��� �����°��������� �����ؾ��Ѵ�.

			UTexture2D* IconImage = DragWidget->GetIconImage();
			SkillInfo* Info = DragWidget->GetSkillInfo();
			int32 SkillLevel = DragWidget->GetSkillLevel();
			
			SlotListWidget->Check(Info->iSkillNumber, HitIndex); // �ߺ�ó��

			SlotListWidget->SetIconImage(IconImage, HitIndex);
			SlotListWidget->SetSkillInfo(Info, SkillLevel, HitIndex);
		}
		else
		{
			// ����ȭ�鿡 �̹����� ������
			SkillInfo* Info = DragWidget->GetSkillInfo();

			SlotListWidget->Check(Info->iSkillNumber, HitIndex);
		}

		GameCanvas->RemoveChild(DragWidget);

		WidgetTree->RemoveWidget(DragWidget);
		DragWidget = nullptr;
	}
	else if (ItemDragWidget)
	{
		FVector2D	PixelPosition, ViewportMin, ViewportMax;

		USlateBlueprintLibrary::LocalToViewport(GetWorld(), MyGeometry, FVector2D(0.f, 0.f), PixelPosition, ViewportMin);
		USlateBlueprintLibrary::LocalToViewport(GetWorld(), MyGeometry, MyGeometry.GetLocalSize(), PixelPosition, ViewportMax);

		FVector2D	MousePos = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());

		MousePos -= ViewportMin;

		int32	HitIndex = SlotListItemWidget->CollisionSlot(MousePos);

		if (HitIndex != -1 && (ItemDragWidget->GetItemData()->GetItemType() == ItemType::IT_POTION) )
		{
			UTexture2D* IconImage = ItemDragWidget->GetItemIconImage();
			UItemData* ItemData = ItemDragWidget->GetItemData();

			SlotListItemWidget->Check(ItemData, HitIndex);

			SlotListItemWidget->SetItemData(ItemData, HitIndex);
			SlotListItemWidget->SetIconImage(IconImage, HitIndex);

			InventoryWidget->Clear();
		}
		else if (HitIndex != -1 && (ItemDragWidget->GetItemData()->GetItemType() != ItemType::IT_POTION))
		{
			InventoryWidget->Clear();
		}

		else if (HitIndex == -1 && (ItemDragWidget->GetDragItemType() == DragItemType::InventoryItem))
		{
			// �κ��丮���� ������ �������� �������� �����̴�.
		}

		else if(ItemDragWidget->GetDragItemType() == DragItemType::SlotItem)
		{
			UItemData* ItemData = ItemDragWidget->GetItemData();

			SlotListItemWidget->Check(ItemData, HitIndex);

			InventoryWidget->Clear();
		}

		GameCanvas->RemoveChild(ItemDragWidget);

		WidgetTree->RemoveWidget(ItemDragWidget);
		ItemDragWidget = nullptr;
	}
}

FReply UGameWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply	result = Super::NativeOnMouseMove(InGeometry, InMouseEvent);

	m_MousePos = InMouseEvent.GetScreenSpacePosition();

	m_MousePos = USlateBlueprintLibrary::AbsoluteToLocal(InGeometry, m_MousePos);

	return result;
}

void UGameWidget::OpenStoreWidget()
{
	if (StoreWidget->GetVisibility() == ESlateVisibility::Collapsed)
		StoreWidget->SetVisibility(ESlateVisibility::Visible);
}

void UGameWidget::OpenSkillWidget()
{
	if (SkillWidget->GetVisibility() == ESlateVisibility::Collapsed)
		SkillWidget->SetVisibility(ESlateVisibility::Visible);
}

void UGameWidget::OpenQuestWidget()
{
	if (QuestWidget->GetVisibility() == ESlateVisibility::Collapsed)
		QuestWidget->SetVisibility(ESlateVisibility::Visible);
}

void UGameWidget::OpenInventoryWidget()
{
	if (InventoryWidget->GetVisibility() == ESlateVisibility::Collapsed)
		InventoryWidget->SetVisibility(ESlateVisibility::Visible);
}

void UGameWidget::OpenTradeWidget()
{
	if (TradeWidget->GetVisibility() == ESlateVisibility::Collapsed)
		TradeWidget->SetVisibility(ESlateVisibility::Visible);

	InventoryWidget->SetTrade(true);
}

void UGameWidget::OpenPlayerInfoWidget(AUserCharacter* pPlayer)
{
	if(PlayerInfoWidget->GetVisibility() == ESlateVisibility::Collapsed)
		PlayerInfoWidget->SetVisibility(ESlateVisibility::Visible);

	PlayerInfoWidget->SetOtherPlayer(pPlayer);

	PlayerInfoWidget->SetLocationFromMouse(m_MousePos);

	/*const FGeometry& geometry = GetCachedGeometry();

	float	MouseX, MouseY;
	if (GetWorld()->GetFirstPlayerController()->GetMousePosition(MouseX, MouseY))
	{
		FVector2D	MousePos;
		MousePos.X = MouseX;
		MousePos.Y = MouseY;

		MousePos = USlateBlueprintLibrary::AbsoluteToLocal(geometry, MousePos);

		PlayerInfoWidget->SetLocationFromMouse(m_MousePos);
	}*/
}

void UGameWidget::OpenTradeRequestWidget()
{
	if (TradeRequestWidget->GetVisibility() == ESlateVisibility::Collapsed)
	{
		TradeRequestWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void UGameWidget::OpenGroupWidget()
{
	if (GroupWidget->GetVisibility() == ESlateVisibility::Collapsed)
	{
		GroupWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void UGameWidget::OpenGroupRequestWidget()
{
	if (GroupRequestWidget->GetVisibility() == ESlateVisibility::Collapsed)
	{
		GroupRequestWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void UGameWidget::OpenMainPortalWidget()
{
	if (MainPortalWidget->GetVisibility() == ESlateVisibility::Collapsed)
	{
		MainPortalWidget->SetVisibility(ESlateVisibility::Visible);
	}
}	
						
void UGameWidget::ClosePlayerInfoWidget()
{
	if (PlayerInfoWidget->GetVisibility() == ESlateVisibility::Visible)
	{
		PlayerInfoWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UGameWidget::SetExpPercent(float Percent)
{
	ExpBar->SetPercent(Percent);
}