// Fill out your copyright notice in the Description page of Project Settings.


#include "TradeWidget.h"
#include "../Store/ItemData.h"
#include "../UICell.h"
#include "../../NetStream.h"
#include "../../PacketSession.h"
#include "../../NetworkManager.h"
#include "../../GameServerManager.h"
#include "../../MainGameInstance.h"
#include "../ItemInfoWidget.h"
#include "../GameWidget.h"

#include "../../Chatting/ChattingWidget.h"
#include "../Inventory/InventoryWidget.h"
#include "../Inventory/InventoryItemWidget.h"
#include "../../GameMode/LectureGameGameModeBase.h"

void UTradeWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PlayerView = Cast<UTileView>(GetWidgetFromName(TEXT("PlayerView")));
	PlayerCell = Cast<UTileView>(GetWidgetFromName(TEXT("PlayerBack")));
	OtherPlayerView = Cast<UTileView>(GetWidgetFromName(TEXT("OtherPlayerView")));
	OtherPlayerCell = Cast<UTileView>(GetWidgetFromName(TEXT("OtherPlayerBack")));
	PlayerName = Cast<UTextBlock>(GetWidgetFromName(TEXT("PlayerName")));
	OtherPlayerName = Cast<UTextBlock>(GetWidgetFromName(TEXT("OtherPlayerName")));
	ExchangeButton = Cast<UButton>(GetWidgetFromName(TEXT("Exchange")));
	OtherUserTradeCheck = Cast<UImage>(GetWidgetFromName(TEXT("OtherUserCheck")));
	CloseButton = Cast<UButton>(GetWidgetFromName(TEXT("Close")));

	m_pItemInfo = Cast<UItemInfoWidget>(GetWidgetFromName(TEXT("ItemInfo")));

	ExchangeButton->OnClicked.AddDynamic(this, &UTradeWidget::Exchange);
	CloseButton->OnClicked.AddDynamic(this, &UTradeWidget::Cancel);

	PlayerView->OnItemDoubleClicked().AddUObject(this, &UTradeWidget::ItemDoubleClick);

	PlayerView->OnItemIsHoveredChanged().AddUObject(this, &UTradeWidget::Item_Hovered);
	OtherPlayerView->OnItemIsHoveredChanged().AddUObject(this, &UTradeWidget::Item_Hovered);

	for (int i = 0; i < 4; ++i) //7로 하면 우측에 스크롤바가 생성이 되어 기존 위치가 조금씩 촘촘해짐....
	{
		for (int j = 0; j < 5; ++j)
		{
			UUICell* Cell = NewObject<UUICell>(this, UUICell::StaticClass());

			PlayerCell->AddItem(Cell);
			OtherPlayerCell->AddItem(Cell);
		}
	}

	PlayerReady = false;
	OtherPlayerReady = false;
}

void UTradeWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (m_bMouseHovered)
	{
		FVector2D	PixelPosition, ViewportMin, ViewportMax;

		USlateBlueprintLibrary::LocalToViewport(GetWorld(), MyGeometry, FVector2D(0.f, 0.f), PixelPosition, ViewportMin);
		USlateBlueprintLibrary::LocalToViewport(GetWorld(), MyGeometry, MyGeometry.GetLocalSize(), PixelPosition, ViewportMax);

		FVector2D	MousePos = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());

		MousePos -= ViewportMin;

		UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(m_pItemInfo->Slot);

		CanvasSlot->SetPosition(FVector2D(MousePos.X + 3.f, MousePos.Y + 3.f));
	}
}

void UTradeWidget::AddItem(UItemData* Item)
{
	UItemData* pItem = NewObject<UItemData>(this, UItemData::StaticClass());

	pItem->SetItemName(Item->GetItemName());
	pItem->SetItemGrade(Item->GetItemGrade());
	pItem->SetItemType(Item->GetItemType());
	pItem->SetPrice(Item->GetPrice());
	pItem->SetSell(Item->GetSell());
	pItem->SetIconTexture(Item->GetIconTexture());
	pItem->SetItemIndex(Item->GetItemIndex());

	for (int32 i = 0; i < Item->GetOptionCount(); ++i)
	{
		pItem->AddOption(Item->GetOption(i).Type, Item->GetOption(i).Option);
	}

	PlayerView->AddItem(pItem);
	// 서버에 아이템 정보를 보내주도록 한다.

	NetStream	stream;
	uint8	strPacket[PACKET_LENGTH] = {};
	stream.SetBuffer(strPacket);
	// =======================================
	stream.Write(&OtherPlayerCharacterNumber, sizeof(int32));

	const char* ItemName = pItem->GetNameData();
	char Name[ITEMNAME_SIZE] = {};
	FMemory::Memcpy(Name, ItemName, ITEMNAME_SIZE);
	stream.Write(Name, ITEMNAME_SIZE);

	EItemGrade Grade = pItem->GetItemGrade();
	stream.Write(&Grade, sizeof(EItemGrade));
	ItemType eType = pItem->GetItemType();
	stream.Write(&eType, sizeof(ItemType));
	int32 iPrice = pItem->GetPrice();
	stream.Write(&iPrice, sizeof(int32));
	int32 iSell = pItem->GetSell();
	stream.Write(&iSell, sizeof(int32));
	
	int32 iOptionCount = pItem->GetOptionCount();
	stream.Write(&iOptionCount, sizeof(int32));
	for (int32 i = 0; i < iOptionCount; ++i)
	{
		FItemOptionData Option = pItem->GetOption(i);

		stream.Write(&Option, sizeof(Option));
	}

	// =======================================
	PacketSession* pSession = GameServerManager::GetInst()->GetSession();
	pSession->Write(GP_TRADEADD, stream.GetLength(), strPacket);
}

void UTradeWidget::AddItem(const FString& ItemName, EItemGrade Grade, ItemType Type, int32 Price, int32 Sell, const TArray<FItemOption>& OptionArray)
{
	UItemData* pItem = NewObject<UItemData>(this, UItemData::StaticClass());

	pItem->SetItemName(ItemName);
	pItem->SetItemGrade(Grade);
	pItem->SetItemType(Type);
	pItem->SetPrice(Price);
	pItem->SetSell(Sell);
	
	UMainGameInstance* pGameInst = Cast<UMainGameInstance>(GetWorld()->GetGameInstance());
	
	const FIconPathInfo* pPathInfo = pGameInst->GetIconPath(ItemName);

	if (pPathInfo)
	{
		pItem->SetIconTexture(pPathInfo->Path);
	}

	for (const FItemOption& OptionData : OptionArray)
	{
		pItem->AddOption(OptionData.Type, OptionData.Option);
	}

	OtherPlayerView->AddItem(pItem);
}

void UTradeWidget::DeleteItem(UItemData* Item)
{
	NetStream	stream;
	uint8	strPacket[PACKET_LENGTH] = {};
	stream.SetBuffer(strPacket);
	// =======================================
	stream.Write(&OtherPlayerCharacterNumber, sizeof(int32));

	int32 Index = PlayerView->GetIndexForItem(Item);
	stream.Write(&Index, sizeof(int32));

	// =======================================
	PacketSession* pSession = GameServerManager::GetInst()->GetSession();
	pSession->Write(GP_TRADEDELETE, stream.GetLength(), strPacket);


	PlayerView->RemoveItem(Item);
}

void UTradeWidget::DeleteItem(int32 Index)
{
	UItemData* pItem = (UItemData*)OtherPlayerView->GetItemAt(Index);

	OtherPlayerView->RemoveItem(pItem);
}

void UTradeWidget::TradeReady()
{
	FLinearColor LColor;
	FSlateColor SColor;

	if (PlayerReady)
	{
		LColor = FLinearColor(0.190756f, 0.016003f, 0.486979f, 1.f);
		ExchangeButton->SetBackgroundColor(LColor);
	}
	else if (!PlayerReady)
	{
		LColor = FLinearColor(0.8f, 0.8f, 0.8f, 1.f);
		ExchangeButton->SetBackgroundColor(LColor);
	}

	if (OtherPlayerReady)
	{
		SColor = FSlateColor(FLinearColor(0.190756f, 0.016003f, 0.486979f, 1.f));
		OtherUserTradeCheck->SetBrushTintColor(SColor);
	}
	else if (!OtherPlayerReady)
	{
		SColor = FSlateColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
		OtherUserTradeCheck->SetBrushTintColor(SColor);
	}
}

FReply UTradeWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply result = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	ALectureGameGameModeBase* pGameMode = Cast<ALectureGameGameModeBase>(GetWorld()->GetAuthGameMode());
	UGameWidget* pGameWidget = pGameMode->GetGameWidget();
	UInventoryWidget* pInventory = pGameWidget->GetInventoryWidget();

	if (pInventory->GetVisibility() == ESlateVisibility::Visible)
	{
		DeleteItem(m_TradeItemData);
	}

	return result;
}

void UTradeWidget::ItemDoubleClick(UObject* pObj)
{
	UItemData* pData = Cast<UItemData>(pObj);
	ALectureGameGameModeBase* pGameMode = Cast<ALectureGameGameModeBase>(GetWorld()->GetAuthGameMode());
	UGameWidget* pGameWidget = pGameMode->GetGameWidget();
	UInventoryWidget* pInventory = pGameWidget->GetInventoryWidget();

	if (pInventory->IsOpen())
	{

		//pInventory->AddItem(pData);

		PlayerView->RemoveItem(pData);
	}
}

void UTradeWidget::Item_Hovered(UObject* pObj, bool Hovered)
{
	UItemData* Item = Cast<UItemData>(pObj);

	if (IsValid(Item))
	{
		if (Hovered)
		{
			if (m_pItemInfo)
			{
				if (!Item->GetIsEmpty())
				{
					m_pItemInfo->SetItemInfo(Item);
					m_bMouseHovered = true;
					m_TradeItemData = Item;
					m_pItemInfo->SetVisibility(ESlateVisibility::Visible);
				}
			}
		}

		else
		{
			if (m_pItemInfo)
			{
				m_bMouseHovered = false;
				m_TradeItemData = nullptr;
				m_pItemInfo->SetVisibility(ESlateVisibility::Collapsed);
			}
		}
	}
}

void UTradeWidget::Cancel()
{
	if (GetVisibility() == ESlateVisibility::Visible)
	{
		SetVisibility(ESlateVisibility::Collapsed);

		ALectureGameGameModeBase* pGameMode = Cast<ALectureGameGameModeBase>(GetWorld()->GetAuthGameMode());

		UChattingWidget* pChatting = pGameMode->GetGameWidget()->GetChatWidget();
		UInventoryWidget* pInventory = pGameMode->GetGameWidget()->GetInventoryWidget();

		FString string = TEXT("[ Notice ] : 거래가 취소되었습니다.");

		pChatting->AddChatting(string);

		pInventory->SetTrade(false);
		pInventory->CloseInventory();

		int32 PlayerItemCount = PlayerView->GetNumItems();

		for (int32 i = 0; i < PlayerItemCount; ++i)
		{
			UItemData* pItem = Cast<UItemData>(PlayerView->GetItemAt(0));

			PlayerView->RemoveItem(pItem);
		}

		int32 OtherUserItemCount = OtherPlayerView->GetNumItems();
		for (int32 i = 0; i < OtherUserItemCount; ++i)
		{
			UItemData* pItem = Cast<UItemData>(OtherPlayerView->GetItemAt(0));

			OtherPlayerView->RemoveItem(pItem);
		}

		PlayerReady = false;
		OtherPlayerReady = false;

		NetStream stream;
		uint8	strPacket[PACKET_LENGTH] = {};
		stream.SetBuffer(strPacket);

		stream.Write(&OtherPlayerCharacterNumber, sizeof(int32));

		PacketSession* pSession = GameServerManager::GetInst()->GetSession();
		pSession->Write(GP_TRADECANCEL, stream.GetLength(), strPacket);
	}
}

void UTradeWidget::Close()
{
	if (GetVisibility() == ESlateVisibility::Visible)
	{
		SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UTradeWidget::TradeSuccess()
{
	ALectureGameGameModeBase* pGameMode = Cast<ALectureGameGameModeBase>(GetWorld()->GetAuthGameMode());

	UChattingWidget* pChatting = pGameMode->GetGameWidget()->GetChatWidget();
	UInventoryWidget* pInventory = pGameMode->GetGameWidget()->GetInventoryWidget();

	FString string = TEXT("[ Notice ] : 거래가 완료되었습니다..");
	pChatting->AddChatting(string);

	int32 PlayerTradeCount = PlayerView->GetNumItems();
	for (int32 i = 0; i < PlayerTradeCount; ++i)
	{
		UItemData* pItem = Cast<UItemData>(PlayerView->GetItemAt(0));

		pInventory->DeleteItem(pItem);

		PlayerView->RemoveItem(pItem);
	}

	// 상대방의 거래창도 비워줘야한다.
	int32 OtherUserTradeCount = OtherPlayerView->GetNumItems();
	for (int32 i = 0; i < OtherUserTradeCount; ++i)
	{
		UItemData* pItem = Cast<UItemData>(OtherPlayerView->GetItemAt(0));

		pInventory->AddItem(pItem);

		OtherPlayerView->RemoveItem(pItem);
	}

	ReadyClear();
	pInventory->SetTrade(false);
	pInventory->CloseInventory();

	if (GetVisibility() == ESlateVisibility::Visible)
	{
		SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UTradeWidget::Exchange()
{
	// 내 거래창의 아이템을 전부 상대인벤토리에 보내줘야한다.

	if (PlayerReady)
	{
		PlayerReady = false;
	}
	else
	{
		PlayerReady = true;
	}

	TradeReady();


	NetStream stream;
	uint8 strPacket[PACKET_LENGTH] = {};
	stream.SetBuffer(strPacket);

	PacketSession* pSession = GameServerManager::GetInst()->GetSession();

	if (PlayerReady && OtherPlayerReady)
	{
		stream.Write(&OtherPlayerCharacterNumber, sizeof(int32));

		pSession->Write(GP_TRADESUCCESS, stream.GetLength(), strPacket);

		TradeSuccess();
	}

	else if (PlayerReady && !OtherPlayerReady)
	{
		stream.Write(&OtherPlayerCharacterNumber, sizeof(int32));

		pSession->Write(GP_TRADEREADY, stream.GetLength(), strPacket);
	}

	else if(!PlayerReady)
	{
		stream.Write(&OtherPlayerCharacterNumber, sizeof(int32));

		pSession->Write(GP_TRADEREADY, stream.GetLength(), strPacket);
	}
}