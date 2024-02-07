// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidget.h"
#include "InventoryCellWidget.h"
#include "../Store/ItemData.h"
#include "../Store/StoreWidget.h"
#include "../PlayerUI/TradeWidget.h"
#include "../UICell.h"
#include "../../MainGameInstance.h"
#include "../../GameMode/LectureGameGameModeBase.h"
#include "../GameWidget.h"
#include "InventoryItemWidget.h"
#include "../../Chatting/ChattingWidget.h"

void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	m_pTileView = Cast<UTileView>(GetWidgetFromName(TEXT("InventoryList")));
	m_pCloseButton = Cast<UButton>(GetWidgetFromName(TEXT("CloseButton")));
	m_pMoneyText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Money")));
	m_pTileCell = Cast<UTileView>(GetWidgetFromName(TEXT("InventoryCell")));
	m_ItemCountText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Count")));

	m_pItemInfo = Cast<UItemInfoWidget>(GetWidgetFromName(TEXT("ItemInfo")));

	m_pTileView->OnItemDoubleClicked().AddUObject(this, &UInventoryWidget::ItemDoubleClick);
	m_pTileView->OnItemIsHoveredChanged().AddUObject(this, &UInventoryWidget::Item_Hovered);

	m_pCloseButton->OnClicked.AddDynamic(this, &UInventoryWidget::CloseInventory);

	m_InventoryItemExist = false;
	m_bOpen = false;
	m_bOpenTrade = false;
	m_bMouseHovered = false;
	m_bMousePressed = false;
	m_bMousePressStart = false;
	m_bMouseDrag = false;

	m_InventoryItemCount = 0;
	m_InventoryItemCountMax = 1;

	for (int i = 0; i < 6; ++i) //7로 하면 우측에 스크롤바가 생성이 되어 기존 위치가 조금씩 촘촘해짐....
	{
		for (int j = 0; j < 5; ++j)
		{
			UUICell* Cell = NewObject<UUICell>(this, UUICell::StaticClass());

			m_pTileCell->AddItem(Cell);
		}
	}

	FString ItemBackGround("Texture2D'/Game/UI/UIAsset/Tex_background_01.Tex_background_01_0'");
	InventoryItemBackGround = LoadObject<UTexture2D>(nullptr, *ItemBackGround);

	int32 iCount = 0;
	for (int i = 0; i < 5; ++i)
	{
		for (int j = 0; j < 6; ++j)
		{
			UItemData* Item = NewObject<UItemData>(this, UItemData::StaticClass());

			int32 iIndex = i * 6 + j;

			Item->SetItemIndex(iIndex);
			Item->SetIsEmpty(true);
			Item->SetIconTexture(InventoryItemBackGround);

			m_pTileView->AddItem(Item);
			iCount++;
		}
	}
	SetInventoryCountMax(iCount);
}

void UInventoryWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
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

	if (m_bMousePressed)
	{
		if (m_bMousePressStart)
		{
			FVector2D	PixelPosition, ViewportMin, ViewportMax;

			USlateBlueprintLibrary::LocalToViewport(GetWorld(), MyGeometry, FVector2D(0.f, 0.f), PixelPosition, ViewportMin);
			USlateBlueprintLibrary::LocalToViewport(GetWorld(), MyGeometry, MyGeometry.GetLocalSize(), PixelPosition, ViewportMax);

			PressStartPos = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());

			PressStartPos -= ViewportMin;

			m_bMousePressStart = false;
		}

		else if (!m_bMouseDrag)
		{
			// PressStart로 처음 눌렀을떄 위치와 여기 드래그if로 들어오고 나서의 위치와 비교했을때
			// 다르면 드래그를 했다는 의미
			FVector2D	PixelPosition, ViewportMin, ViewportMax;

			USlateBlueprintLibrary::LocalToViewport(GetWorld(), MyGeometry, FVector2D(0.f, 0.f), PixelPosition, ViewportMin);
			USlateBlueprintLibrary::LocalToViewport(GetWorld(), MyGeometry, MyGeometry.GetLocalSize(), PixelPosition, ViewportMax);

			FVector2D MousePos = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());

			MousePos -= ViewportMin;

			if (MousePos != PressStartPos)
			{
				m_bMouseDrag = true;
			}
		}
	}
}

void UInventoryWidget::ItemDoubleClick(UObject* pObj)
{
	UItemData* pData = Cast<UItemData>(pObj);

	if (m_bOpenTrade)
	{
		PrintViewport(5.f, FColor::Red, TEXT("Inventory Item Double Click--------"));

		ALectureGameGameModeBase* pGameMode = Cast<ALectureGameGameModeBase>(GetWorld()->GetAuthGameMode());
		if (pGameMode)
		{
			UGameWidget* pGameWidget = pGameMode->GetGameWidget();
			if (!pGameWidget)
				return;

			pGameWidget->GetTradeWidget()->AddItem(pData);

			// 여기서 인벤 아이템을 빼주면 거래를 취소했을 경우 해당 아이템을 인벤토리로 다시 되돌려줘야 한다.
			m_pTileView->RemoveItem(pObj);
		}
	}
}

void UInventoryWidget::CloseInventory()
{
	ALectureGameGameModeBase* pGameMode = Cast<ALectureGameGameModeBase>(GetWorld()->GetAuthGameMode());
	UStoreWidget* pStore = pGameMode->GetGameWidget()->GetStoreWidget();
	if (pStore->GetVisibility() == ESlateVisibility::Visible)
	{
		pStore->SetVisibility(ESlateVisibility::Collapsed);
		pStore->Close();
	}
	SetVisibility(ESlateVisibility::Collapsed);
	m_bOpen = false;
}

void UInventoryWidget::AddItem(const FString& ItemName, EItemGrade Grade, ItemType Type, int32 Price, int32 Sell, const TArray<FItemOption>& OptionArray)
{
	if ((m_InventoryItemCount + 1) > m_InventoryItemCountMax)
	{
		PrintViewport(5.f, FColor::Blue, FString::Printf(TEXT("인벤토리가 꽉 찼습니다...")));

		return;
	}

	const TArray<UObject*> pData = m_pTileView->GetListItems();

	int iIndex = -1;
	for (int i = 0; i < pData.Num(); ++i)
	{
		// 최대 갯수에 맞춰 넣는것이 아닌 중간에 빈공간이 있을경우 그곳에 넣도록 해야할듯....
		UItemData* Item = Cast<UItemData>(pData[i]);

		if (Item->GetIsEmpty())
		{
			iIndex = i;
			break;
		}
	}

	UItemData* Item = nullptr;

	// 중간에 아이템빈칸이 있을수도 있으니(버리거나 팔거나)
	if (iIndex != m_InventoryItemCount)
	{
		Item = Cast<UItemData>(m_pTileView->GetItemAt(iIndex));
	}
	else if (iIndex == m_InventoryItemCount)
	{
		Item = Cast<UItemData>(m_pTileView->GetItemAt(m_InventoryItemCount));
	}

	Item->SetItemName(ItemName);
	Item->SetItemGrade(Grade);
	Item->SetItemType(Type);
	Item->SetPrice(Price);
	Item->SetSell(Sell);
	Item->SetIsEmpty(false);

	UMainGameInstance* pGameInst = Cast<UMainGameInstance>(GetWorld()->GetGameInstance());

	const FIconInfo* pIconInfo = pGameInst->GetIconImagePath(ItemName);

	if (pIconInfo)
		Item->SetIconTexture(pIconInfo->Texture);

	for (auto& OptionData : OptionArray)
	{
		Item->AddOption(OptionData.Type, OptionData.Option);
	}

	m_InventoryItemCount++;
	SetInventoryCount(m_InventoryItemCount);

	if (IsExist())
	{
		UInventoryItemWidget* pItemWidget = Cast<UInventoryItemWidget>(m_pTileView->GetEntryWidgetFromItem(Item));
		pItemWidget->SetData(Item);
	}

	// 현재 아이템 정보로 가져오는건데 이거는....
	// 아직 InventoryItemWidget이(안열었기 때문에) 생성이 안되었기 떄문에 안나온것이다.
	// Inventory을 열어야 위젯들이 생성시작을 하기떄문에....
	//UInventoryItemWidget* pItemWidget = Cast<UInventoryItemWidget>(m_pTileView->GetEntryWidgetFromItem(Item));
}

void UInventoryWidget::AddItem(UItemData* Data)
{
	if ((m_InventoryItemCount + 1) > m_InventoryItemCountMax)
	{
		PrintViewport(5.f, FColor::Blue, FString::Printf(TEXT("인벤토리가 꽉 찼습니다...")));

		return;
	}

	const TArray<UObject*> pData = m_pTileView->GetListItems();

	int iIndex = -1;
	for (int i = 0; i < pData.Num(); ++i)
	{
		UItemData* Item = Cast<UItemData>(pData[i]);

		if (Item->GetIsEmpty())
		{
			iIndex = i;
			break;
		}
	}

	UItemData* Item = nullptr;

	if (iIndex != m_InventoryItemCount)
	{
		Item = Cast<UItemData>(m_pTileView->GetItemAt(iIndex));
	}
	else if (iIndex == m_InventoryItemCount)
	{
		Item = Cast<UItemData>(m_pTileView->GetItemAt(m_InventoryItemCount));
	}

	Item->SetItemName(Data->GetItemName());
	Item->SetItemGrade(Data->GetItemGrade());
	Item->SetItemType(Data->GetItemType());
	Item->SetPrice(Data->GetPrice());
	Item->SetSell(Data->GetSell());
	Item->SetIsEmpty(false);

	UMainGameInstance* pGameInst = Cast<UMainGameInstance>(GetWorld()->GetGameInstance());

	const FIconPathInfo* pPathInfo = pGameInst->GetIconPath(Data->GetItemName());

	if (pPathInfo)
		Item->SetIconTexture(pPathInfo->Path);

	int32 OptionCount = Data->GetOptionCount();
	for (int32 i = 0; i < OptionCount; ++i)
	{
		FItemOptionData OptionData = Data->GetOption(i);
		Item->AddOption(OptionData.Type, OptionData.Option);
	}

	m_InventoryItemCount++;
	SetInventoryCount(m_InventoryItemCount);

	if (GetVisibility() == ESlateVisibility::Visible)
	{
		UInventoryItemWidget* pItemWidget = Cast<UInventoryItemWidget>(m_pTileView->GetEntryWidgetFromItem(Item));
		pItemWidget->SetData(Item);
	}

}

void UInventoryWidget::DeleteItem(UItemData* Data)
{
	if (m_InventoryItemCount < 1)
	{
		PrintViewport(3.f, FColor::Blue, FString::Printf(TEXT("인벤토리 공백상태의 삭제요청.....")));

		return;
	}

	const TArray<UObject*> pData = m_pTileView->GetListItems();

	for (int i = 0; i < pData.Num(); ++i)
	{
		UItemData* Item = Cast<UItemData>(pData[i]);

		if (!Item->GetIsEmpty())
		{
			if (Item->GetItemIndex() == Data->GetItemIndex())
			{
				Item->Clear();

				UInventoryItemWidget* pItemWidget = Cast<UInventoryItemWidget>(m_pTileView->GetEntryWidgetFromItem(Item));
				pItemWidget->Clear(InventoryItemBackGround);

				m_InventoryItemCount--;
				SetInventoryCount(m_InventoryItemCount);

				PrintViewport(3.f, FColor::Blue, FString::Printf(TEXT("%s 아이템이 제거되었습니다....."), *Item->GetItemName()));

				break;
			}
		}
	}

}

bool UInventoryWidget::IndexMoveItem(int32 Source, int32 Dest)
{
	UItemData* SourceItem = Cast<UItemData>(m_pTileView->GetItemAt(Source));
	UItemData* DestItem = Cast<UItemData>(m_pTileView->GetItemAt(Dest));

	if (!DestItem->GetIsEmpty())
	{
		UItemData* Tempo = NewObject<UItemData>(this, UItemData::StaticClass());
		{
			Tempo->SetItemName(DestItem->GetItemName());
			Tempo->SetItemGrade(DestItem->GetItemGrade());
			Tempo->SetItemType(DestItem->GetItemType());
			Tempo->SetPrice(DestItem->GetPrice());
			Tempo->SetSell(DestItem->GetSell());
			Tempo->SetIconTexture(DestItem->GetIconTexture());

			Tempo->ClearOption();
			for (int32 i = 0; i < DestItem->GetOptionCount(); ++i)
			{
				Tempo->AddOption(DestItem->GetOption(i).Type, DestItem->GetOption(i).Option);
			}
		}
		//FMemory::Memcpy(Tempo, DestItem, sizeof(UItemData));

		{
			DestItem->SetItemName(SourceItem->GetItemName());
			DestItem->SetItemGrade(SourceItem->GetItemGrade());
			DestItem->SetItemType(SourceItem->GetItemType());
			DestItem->SetPrice(SourceItem->GetPrice());
			DestItem->SetSell(SourceItem->GetSell());
			DestItem->SetIconTexture(SourceItem->GetIconTexture());

			DestItem->ClearOption();
			for (int32 i = 0; i < SourceItem->GetOptionCount(); ++i)
			{
				DestItem->AddOption(SourceItem->GetOption(i).Type, SourceItem->GetOption(i).Option);
			}
		}

		SourceItem->SetItemName(Tempo->GetItemName());
		SourceItem->SetItemGrade(Tempo->GetItemGrade());
		SourceItem->SetItemType(Tempo->GetItemType());
		SourceItem->SetPrice(Tempo->GetPrice());
		SourceItem->SetSell(Tempo->GetSell());
		SourceItem->SetIconTexture(Tempo->GetIconTexture());

		SourceItem->ClearOption();
		for (int32 i = 0; i < Tempo->GetOptionCount(); ++i)
		{
			SourceItem->AddOption(Tempo->GetOption(i).Type, Tempo->GetOption(i).Option);
		}

		Tempo->MarkPendingKill();

		return true;
	}
	else // 비어있을 경우
	{
		DestItem->SetItemName(SourceItem->GetItemName());
		DestItem->SetItemGrade(SourceItem->GetItemGrade());
		DestItem->SetItemType(SourceItem->GetItemType());
		DestItem->SetPrice(SourceItem->GetPrice());
		DestItem->SetSell(SourceItem->GetSell());
		DestItem->SetIconTexture(SourceItem->GetIconTexture());
		DestItem->SetIsEmpty(false);

		DestItem->ClearOption();
		for (int32 i = 0; i < SourceItem->GetOptionCount(); ++i)
		{
			DestItem->AddOption(SourceItem->GetOption(i).Type, SourceItem->GetOption(i).Option);
		}

		SourceItem->Clear();

		return false;
	}

	return false;
}

FReply UInventoryWidget::NativeOnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	// 아이템있으면 안들어오고 오른쪽 버튼은 되는데 왼쪽 버튼만 안먹힘...
	FReply result = Super::NativeOnMouseButtonDown(MyGeometry, MouseEvent);

	ALectureGameGameModeBase* pGameMode = Cast<ALectureGameGameModeBase>(GetWorld()->GetAuthGameMode());
	UGameWidget* pGameWidget = pGameMode->GetGameWidget();
	UStoreWidget* pStore = pGameWidget->GetStoreWidget();
	UChattingWidget* pChattingWidget = pGameWidget->GetChatWidget();

	// 어떠한 Ui가 같이 열려있는야에 따라 다른 작업 진행
	if (m_bOpenTrade)
	{
		UTradeWidget* pTradeWidget = pGameWidget->GetTradeWidget();

		if (pTradeWidget->GetPlayerReady() ==  false)
		{
			pTradeWidget->AddItem(m_UIItemData);
		}
	}

	else if (pStore->GetVisibility() == ESlateVisibility::Visible)
	{
		// 상점 판매
		if (m_UIItemData)
		{
			int32 SellPrice = m_UIItemData->GetSell();

			UInventoryItemWidget* pItemWidget = Cast<UInventoryItemWidget>(m_pTileView->GetEntryWidgetFromItem(m_UIItemData));

			EItemGrade Grade = m_UIItemData->GetItemGrade();
			FString strGrade;
			switch (Grade)
			{
			case EItemGrade::Normal:
				strGrade = "Normal";
				break;
			case EItemGrade::Rare:
				strGrade = "Rare";
				break;
			case EItemGrade::Epic:
				strGrade = "Epic";
				break;
			case EItemGrade::Unique:
				strGrade = "Unique";
				break;
			case EItemGrade::Legendary:
				strGrade = "Legendary";
				break;
			}

			FString Money = FString::Printf(TEXT("%s을 팔아서 %d"), *m_UIItemData->GetItemName(), SellPrice);
			pChattingWidget->AddChatting(TEXT("[ Notice ] : (") + strGrade + TEXT(") ") + Money + TEXT("$$ 를 얻었습니다..."), true);

			pItemWidget->Clear(InventoryItemBackGround);
			m_UIItemData->Clear();
			
			SetGold(SellPrice);

			//int32 iItemIndex = m_UIItemData->GetItemIndex();
		}
	}

	else
	{
		// 버리는것.
		if (!m_UIItemData)
		{
			return result;
		}

		UInventoryItemWidget* pItemWidget = Cast<UInventoryItemWidget>(m_pTileView->GetEntryWidgetFromItem(m_UIItemData));
		EItemGrade Grade = m_UIItemData->GetItemGrade();
		FString strGrade;
		switch (Grade)
		{
		case EItemGrade::Normal:
			strGrade = "Normal";
			break;
		case EItemGrade::Rare:
			strGrade = "Rare";
			break;
		case EItemGrade::Epic:
			strGrade = "Epic";
			break;
		case EItemGrade::Unique:
			strGrade = "Unique";
			break;
		case EItemGrade::Legendary:
			strGrade = "Legendary";
			break;
		}

		FString DeleteItem = FString::Printf(TEXT("%s을 버렸습니다..."), *m_UIItemData->GetItemName());
		pChattingWidget->AddChatting(TEXT("[ Notice ] : (") + strGrade + TEXT(") ") + DeleteItem, true);

		pItemWidget->Clear(InventoryItemBackGround);
		m_UIItemData->Clear();



	}

	return result;
}

void UInventoryWidget::Item_Hovered(UObject* pObj, bool Hovered)
{
	UItemData* Item = Cast<UItemData>(pObj);

	// Press일떄 Hovered한번 → Release할때 이전 동일할게 나오는거 한번 그 이후 서로 다른 값인 Hovered

	if (m_DragItemData)
	{
		FString Tempo1 = Item->GetItemName();
		FString Tempo2 = m_DragItemData->GetItemName();

		if (Tempo1.Compare(Tempo2) != 0)
		{
			UInventoryItemWidget* pItemWidget = Cast<UInventoryItemWidget>(m_pTileView->GetEntryWidgetFromItem(Item));
			UInventoryItemWidget* pDragItemWidget = Cast<UInventoryItemWidget>(m_pTileView->GetEntryWidgetFromItem(m_DragItemData));

			// true면 2개 서로 교환 / false이면 1개만 이동
			bool Result = IndexMoveItem(m_DragItemData->GetItemIndex(), Item->GetItemIndex());

			if (Result)
			{
				pItemWidget->SetData(Item); 
				pDragItemWidget->SetData(m_DragItemData);
			}
			else
			{
				pItemWidget->SetData(Item);
				pDragItemWidget->Clear(InventoryItemBackGround);
			}

			// 사실상 Listview의 내부에 있는 TArray를 바꾸지 않는 이상 pObj는 원래 값을 계속 줄것이다.
			// 드래그 넘버도 사실상 의미 없다. UObject에 함수로 존재함...

			// 그냥 미리 30개정도를 넣어주는것이 어떤지? 그래서 빈 공간에다 둘떄 빈 공간인지를 확인을 가능하니까
			// 지금 지우면 뒤에 있던 ItemData들이 한 칸씩 당겨진다...
			// 그래서 미리 UItemData을 다 넣어주고 그냥 값들만 변경을 해주는것이 어떤지 ?
			// 지우지는 말고 그렇다면 종료할떄 생성하는 Data들 다 해제 해줘야 하는건지?

			// UItemData로 게임 시작시 인벤토리에 넣어준다. 
			// 그렇지만 InventoryItemWidget에서 이 Widget이 생성될때 Entry일떄 Setdata가 이루어진다.
			// 그래서 Inventory을 처음 열었을때 그때 SetData가 시작된다.


			//const TArray<UObject*> pData = m_pTileView->GetListItems();
			//UItemData* Item2 = Cast<UItemData>(m_pTileView->GetItemAt(m_DragItemData->GetItemIndex()));
			// 현재 UItemData를 생성하고 FMemory의 Memcpy를 하면 원본 값이 변경되면 이것도 또한 변경된다.........

			// 현재 여기로 들어오기전에 dragItem과 ObjectItem이 같은흐름이 2번 들어온다.....

			Clear();
		}

		else if(m_DragItemData->GetItemIndex() == Item->GetItemIndex() && !m_bMousePressed)
		{

			int32 a = 10;
		}
	}

	if (IsValid(Item))
	{
		if (Hovered)
		{
			if (m_pItemInfo)
			{
				if (!Item->GetIsEmpty())
				{
					m_bMouseHovered = true;

					m_pItemInfo->SetItemInfo(Item);

					m_UIItemData = Item;

					m_pItemInfo->SetVisibility(ESlateVisibility::Visible);
				}
			}
		}

		else
		{
			if (m_pItemInfo)
			{
				m_bMouseHovered = false;

				m_UIItemData = nullptr;

				m_pItemInfo->SetVisibility(ESlateVisibility::Collapsed);
			}
		}
	}
}
