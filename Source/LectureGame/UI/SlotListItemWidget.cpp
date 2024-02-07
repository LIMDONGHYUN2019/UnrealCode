// Fill out your copyright notice in the Description page of Project Settings.


#include "SlotListItemWidget.h"
#include "Blueprint/WidgetTree.h"



void USlotListItemWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UButton* Button1 = Cast<UButton>(GetWidgetFromName(TEXT("Slot1")));
	SlotArray.Add(Button1);
	Button1->OnHovered.AddDynamic(this, &USlotListItemWidget::Item1_Hovered);
	Button1->OnUnhovered.AddDynamic(this, &USlotListItemWidget::Item1_UnHovered);
	Button1->OnPressed.AddDynamic(this, &USlotListItemWidget::Item1_Pressed);
	Button1->OnReleased.AddDynamic(this, &USlotListItemWidget::Item1_Released);

	UBorder* Icon1 = Cast<UBorder>(GetWidgetFromName(TEXT("Icon1")));
	SlotIconArray.Add(Icon1);

	UButton* Button2 = Cast<UButton>(GetWidgetFromName(TEXT("Slot2")));
	SlotArray.Add(Button2);
	Button2->OnHovered.AddDynamic(this, &USlotListItemWidget::Item2_Hovered);
	Button2->OnUnhovered.AddDynamic(this, &USlotListItemWidget::Item2_UnHovered);
	Button2->OnPressed.AddDynamic(this, &USlotListItemWidget::Item2_Pressed);
	Button2->OnReleased.AddDynamic(this, &USlotListItemWidget::Item2_Released);

	UBorder* Icon2 = Cast<UBorder>(GetWidgetFromName(TEXT("Icon2")));
	SlotIconArray.Add(Icon2);

	TArray<UButton*>	BtnArray;
	BtnArray.Add(Button1);
	BtnArray.Add(Button2);

	for (int32 i = 0; i < 2; ++i)
	{
		UCanvasPanelSlot* ButtonSlot = Cast<UCanvasPanelSlot>(BtnArray[i]->Slot);

		FVector2D Pos = ButtonSlot->GetPosition();
		FVector2D Size = ButtonSlot->GetSize();

		// Canvas의 크기를 구한다.
		//FAnchors Anchor = ButtonSlot->GetAnchors();

		//Pos.X += (CanvasSize.X * 0.5f);
		//Pos.Y += (CanvasSize.Y * 0.5f);
		Pos.X += 100.f;
		Pos.Y += 50.f;

		// 왼쪽 상단의 위치를 구한다.
		FVector2D Alignment = ButtonSlot->GetAlignment();

		Pos.X = Pos.X - Alignment.X * Size.X;
		Pos.Y = Pos.Y - Alignment.Y * Size.Y;

		Rectangle	rc;

		rc.Pos = Pos;
		rc.Size = Size;

		SlotRectArray.Add(rc);
	}

	for (int i = 0; i < 2; ++i)
	{
		ItemDataArray.Add(nullptr);
		m_ItemIconImageArray.Add(nullptr);
	}

	//CanvasSlot->SetPosition(FVector2D(MousePos.X, MousePos.Y));//
	//CanvasSlot->SetAlignment(FVector2D(0.5f, 0.5f));
	//CanvasSlot->SetSize(FVector2D(70.f, 70.f));

	ItemInfoWidget = Cast<UItemInfoWidget>(GetWidgetFromName(TEXT("ItemInfo")));

	m_bMouseHovered = false;
	m_bMousePressed = false;
	m_bMousePressStart = false;
	m_bMouseDrag = false;
	m_DragSlotIndex = -1;
}

void USlotListItemWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (m_bMouseHovered)
	{
		FVector2D	PixelPosition, ViewportMin, ViewportMax;

		USlateBlueprintLibrary::LocalToViewport(GetWorld(), MyGeometry, FVector2D(0.f, 0.f), PixelPosition, ViewportMin);
		USlateBlueprintLibrary::LocalToViewport(GetWorld(), MyGeometry, MyGeometry.GetLocalSize(), PixelPosition, ViewportMax);

		FVector2D	MousePos = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());

		MousePos -= ViewportMin;

		UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(ItemInfoWidget->Slot);

		FVector2D Pos = CanvasSlot->GetPosition();
		FVector2D Size = CanvasSlot->GetSize();

		CanvasSlot->SetPosition(FVector2D(MousePos.X + 10.f, MousePos.Y + 3.f - Size.Y));
	}

	if (m_bMousePressed)
	{
		// 누른 후에 마우스가 움직인다면 그때부터는 드래그가 된다.
		// 드래그 상태인지 아닌지를 판단하고 드래그 상태라면 클릭을 막아야 한다.

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

void USlotListItemWidget::ComputeWorldPos(const FVector2D& Pos, const FVector2D& Ratio)
{
	for (int32 i = 0; i < SlotRectArray.Num(); ++i)
	{
		SlotRectArray[i].Pos += Pos;
		//SlotRectArray[i].Size *= Ratio;

		LOG(TEXT("Item %d Slot Pos : %s, size : %s"), i, *SlotRectArray[i].Pos.ToString(), *SlotRectArray[i].Size.ToString());
	}
}

int32 USlotListItemWidget::CollisionSlot(const FVector2D& MousePos)
{
	for (int32 i = 0; i < SlotRectArray.Num(); ++i)
	{
		if (SlotRectArray[i].Pos.X <= MousePos.X && MousePos.X <= SlotRectArray[i].Pos.X + SlotRectArray[i].Size.X &&
			SlotRectArray[i].Pos.Y <= MousePos.Y && MousePos.Y <= SlotRectArray[i].Pos.Y + SlotRectArray[i].Size.Y)
		{
			return i;
		}
	}

	return -1;
}

void USlotListItemWidget::Item1_Hovered()
{
	if (!ItemDataArray[0])
		return;

	ItemInfoWidget->SetVisibility(ESlateVisibility::Visible);

	ItemInfoWidget->SetItemInfo(ItemDataArray[0]);

	m_bMouseHovered = true;
}

void USlotListItemWidget::Item2_Hovered()
{
	if (!ItemDataArray[1])
		return;

	ItemInfoWidget->SetVisibility(ESlateVisibility::Visible);

	ItemInfoWidget->SetItemInfo(ItemDataArray[1]);

	m_bMouseHovered = true;
}

void USlotListItemWidget::Item1_UnHovered()
{
	if (!ItemInfoWidget)
		return;

	m_bMouseHovered = false;

	if (ItemInfoWidget->GetVisibility() == ESlateVisibility::Visible)
		ItemInfoWidget->SetVisibility(ESlateVisibility::Collapsed);
}

void USlotListItemWidget::Item2_UnHovered()
{
	if (!ItemInfoWidget)
		return;

	m_bMouseHovered = false;

	if (ItemInfoWidget->GetVisibility() == ESlateVisibility::Visible)
		ItemInfoWidget->SetVisibility(ESlateVisibility::Collapsed);
}

void USlotListItemWidget::Item1_Pressed()
{
	if (ItemDataArray[0])
	{
		m_bMousePressed = true;
		m_bMousePressStart = true;
		m_DragSlotIndex = 0;

		ItemInfoWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void USlotListItemWidget::Item2_Pressed()
{
	if (ItemDataArray[1])
	{
		m_bMousePressed = true;
		m_bMousePressStart = true;
		m_DragSlotIndex = 1;

		ItemInfoWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void USlotListItemWidget::Item1_Released()
{
	m_bMousePressed = false;
	m_bMouseDrag = false;
	m_DragSlotIndex = -1;

	if (m_bMouseHovered)
		ItemInfoWidget->SetVisibility(ESlateVisibility::Visible);
}

void USlotListItemWidget::Item2_Released()
{
	m_bMousePressed = false;
	m_bMouseDrag = false;
	m_DragSlotIndex = -1;

	if (m_bMouseHovered)
		ItemInfoWidget->SetVisibility(ESlateVisibility::Visible);
}
