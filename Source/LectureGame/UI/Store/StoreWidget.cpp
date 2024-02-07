// Fill out your copyright notice in the Description page of Project Settings.


#include "StoreWidget.h"
#include "StoreItemWidget.h"
#include "ItemData.h"
#include "../ItemInfoWidget.h"
#include "../../MainGameInstance.h"
#include "../../Player/PlayerCharacter.h"
#include "../../GameMode/LectureGameGameModeBase.h"
#include "../Inventory/InventoryWidget.h"
#include "../GameWidget.h"
#include "../../NetSession.h"
#include "../../PacketSession.h"
#include "../../NetStream.h"
#include "../../NetworkManager.h"
#include "../../GameServerManager.h"

void UStoreWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ItemList = Cast<UListView>(GetWidgetFromName(TEXT("StoreList")));
	CloseButton = Cast<UButton>(GetWidgetFromName(TEXT("CloseButton")));

	m_pItemInfo = Cast<UItemInfoWidget>(GetWidgetFromName(TEXT("ItemInfo")));

	ItemList->OnItemDoubleClicked().AddUObject(this, &UStoreWidget::ItemDoubleClick);
	ItemList->OnItemClicked().AddUObject(this, &UStoreWidget::ItemClick);

	ItemList->OnItemIsHoveredChanged().AddUObject(this, &UStoreWidget::Item_Hovered);

	CloseButton->OnClicked.AddDynamic(this, &UStoreWidget::CloseStore);


	FirstOpen = false;
	bOpen = false;
	m_bMouseHovered = false;
}

void UStoreWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
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

void UStoreWidget::ItemClick(UObject* pObj)
{
}

void UStoreWidget::ItemDoubleClick(UObject* pObj)
{
	UItemData* pData = Cast<UItemData>(pObj);

	if (!pData)
		return;

	// 플레이어를 얻어온다.
	APlayerCharacter* pPlayer = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());

	if (!pPlayer)
		return;

	ALectureGameGameModeBase* pGameMode = Cast<ALectureGameGameModeBase>(GetWorld()->GetAuthGameMode());

	if (!pGameMode)
		return;

	UGameWidget* pGameWidget = pGameMode->GetGameWidget();

	if (!pGameWidget)
		return;

	UInventoryWidget* pInventory = pGameWidget->GetInventoryWidget();

	if (!pInventory)
		return;

	if (pData->GetPrice() > pPlayer->GetGold())
		return;

	else if (pInventory->IsFull())
		return;

	// 모든 조건을 통과했으므로 서버로 선택한 아이템을 구매하겠다는 패킷을 보낸다.
	NetStream	stream;
	uint8	strPacket[PACKET_LENGTH] = {};
	stream.SetBuffer(strPacket);

	FItemInfoData	ItemInfo;

	FMemory::Memcpy(ItemInfo.strName, pData->GetNameData(), ITEMNAME_SIZE);
	ItemInfo.Grade = pData->GetItemGrade();
	ItemInfo.Type = pData->GetItemType();
	ItemInfo.Buy = pData->GetPrice();
	ItemInfo.Sell = pData->GetSell();
	int32	OptionCount = pData->GetOptionCount();

	for (int32 i = 0; i < OptionCount; ++i)
	{
		FItemOption	option;

		option.Type = pData->GetOption(i).Type;
		option.Option = pData->GetOption(i).Option;
		ItemInfo.tOption.Add(option);
	}

	stream.Write(ItemInfo.strName, ITEMNAME_SIZE);

	int32 ItemGrade = (int32)ItemInfo.Grade;
	stream.Write(&ItemGrade, sizeof(int32));
	int32 ItemType = (int32)ItemInfo.Type;
	stream.Write(&ItemType, sizeof(int32));
	stream.Write(&ItemInfo.Buy, sizeof(int32));
	stream.Write(&ItemInfo.Sell, sizeof(int32));

	stream.Write(&OptionCount, sizeof(int32));

	for (int32 i = 0; i < OptionCount; ++i)
	{
		stream.Write(&ItemInfo.tOption[i], sizeof(FItemOption));
	}

	PacketSession* GameSession = GameServerManager::GetInst()->GetSession();
	GameSession->Write(GP_INVENTORYADD, stream.GetLength(), strPacket);
}

void UStoreWidget::CloseStore()
{
	SetVisibility(ESlateVisibility::Collapsed);
	bOpen = false;
}

void UStoreWidget::Item_Hovered(UObject* pObj, bool Hovered)
{
	UItemData* Item = Cast<UItemData>(pObj);

	if (IsValid(Item))
	{
		if (Hovered)
		{
			if (m_pItemInfo)
			{
				m_bMouseHovered = true;
				
				m_pItemInfo->SetItemInfo(Item);
				m_pItemInfo->SetVisibility(ESlateVisibility::Visible);

				//PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("Info Position %.1f / %.1f"), InfoPos.X, InfoPos.Y));
			}
		}

		else
		{
			if (m_pItemInfo)
			{
				m_bMouseHovered = false;
				m_pItemInfo->SetVisibility(ESlateVisibility::Collapsed);
			}
		}
	}

}

void UStoreWidget::AddItem(const FString& ItemName, EItemGrade Grade, ItemType Type, int32 Price, int32 Sell, const TArray<FItemOption>& OptionArray)
{
	UItemData* pData = NewObject<UItemData>(this, UItemData::StaticClass());

	pData->SetItemName(ItemName);
	pData->SetItemGrade(Grade);
	pData->SetItemType(Type);
	pData->SetPrice(Price);
	pData->SetSell(Sell);

	pData->SetItemIndex(ItemList->GetNumItems());

	UMainGameInstance* pGameInst = Cast<UMainGameInstance>(GetWorld()->GetGameInstance());

	const FIconPathInfo* pPathInfo = pGameInst->GetIconPath(ItemName);

	if (pPathInfo)
		pData->SetIconTexture(pPathInfo->Path);

	for (auto& OptionData : OptionArray)
	{
		pData->AddOption(OptionData.Type, OptionData.Option);
	}

	ItemList->AddItem(pData);
}
