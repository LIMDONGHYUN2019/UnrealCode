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

			// 왼쪽 상단의 위치를 구한다.
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

			// 왼쪽 상단의 위치를 구한다.
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
			// 거래시스템 드래그로 표현할려했는데 일단 클릭으로 대체
			//UCanvasPanelSlot* TradePanel = Cast<UCanvasPanelSlot>(TradeWidget->Slot);
			//FVector2D Pos = TradePanel->GetPosition();
			//FVector2D Size = TradePanel->GetSize();

			//Pos.X += LocalSize.X * 0.5f;
			//Pos.Y += LocalSize.Y * 0.5f;

			//// 왼쪽 상단의 위치를 구한다.
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
		// 드래그를 시작하는 상태라면 드래그할 스킬 아이콘 위젯을 생성해주도록 한다.
		// 드래그 시작시점인지를 판단해야 한다.
		// 어느것을 드래그하는건지 SkillIcon / Titlebar인지 구분하자.
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
				// 드래그 위젯의 위치를 마우스의 위치를 구해서 지정해준다.
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

	// 여기서 부터 드래그가 중단될떄
	else if (DragWidget)
	{
		// 드래그 하고 있던 마우스를 떼면 마우스의 위치를 구하여 슬롯 안에 들어갔는지 판단한다.
		FVector2D	PixelPosition, ViewportMin, ViewportMax;

		USlateBlueprintLibrary::LocalToViewport(GetWorld(), MyGeometry, FVector2D(0.f, 0.f), PixelPosition, ViewportMin);
		USlateBlueprintLibrary::LocalToViewport(GetWorld(), MyGeometry, MyGeometry.GetLocalSize(), PixelPosition, ViewportMax);

		FVector2D	MousePos = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());

		MousePos -= ViewportMin;

		// 슬롯 리스트의 사각형 정보와 충돌처리를 진행한다.

		// 슬롯리스트 안에 사각형 정보가 있으므로 이 마우스 위치를 이용하여 해당 사각형들과 충돌처리를 해보고 충돌되는게 있다면
		// 해당 사각형 안에 아이콘을 넣어주어야 한다.

		// 슬롯리스트에서 현재 빈 공간에서 드래그가 되고있다.
		int32	HitIndex = SlotListWidget->CollisionSlot(MousePos);

		if (HitIndex != -1)
		{
			LOG(TEXT("Slot Index : %d"), HitIndex);
			// 여기서 스킬창에서 가져온것인지 같은 슬롯에서 가져온것인지부터 구별해야한다.

			UTexture2D* IconImage = DragWidget->GetIconImage();
			SkillInfo* Info = DragWidget->GetSkillInfo();
			int32 SkillLevel = DragWidget->GetSkillLevel();
			
			SlotListWidget->Check(Info->iSkillNumber, HitIndex); // 중복처리

			SlotListWidget->SetIconImage(IconImage, HitIndex);
			SlotListWidget->SetSkillInfo(Info, SkillLevel, HitIndex);
		}
		else
		{
			// 바탕화면에 이미지를 버릴떄
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
			// 인벤토리에서 밖으로 아이템을 던질떄가 문제이다.
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