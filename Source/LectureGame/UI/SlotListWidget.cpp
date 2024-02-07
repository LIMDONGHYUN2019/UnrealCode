// Fill out your copyright notice in the Description page of Project Settings.


#include "SlotListWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Skill/SkillInfoWidget.h"


void USlotListWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UButton* Button1 = Cast<UButton>(GetWidgetFromName(TEXT("Slot1")));
	SlotArray.Add(Button1);
	Button1->OnHovered.AddDynamic(this, &USlotListWidget::Slot1Hovered);
	Button1->OnUnhovered.AddDynamic(this, &USlotListWidget::Slot1UnHovered);
	Button1->OnPressed.AddDynamic(this, &USlotListWidget::Slot1Pressed);
	Button1->OnReleased.AddDynamic(this, &USlotListWidget::Slot1Released);

	UBorder* Icon1 = Cast<UBorder>(GetWidgetFromName(TEXT("Icon1")));
	SlotIconArray.Add(Icon1);

	UButton* Button2 = Cast<UButton>(GetWidgetFromName(TEXT("Slot2")));
	SlotArray.Add(Button2);
	Button2->OnHovered.AddDynamic(this, &USlotListWidget::Slot2Hovered);
	Button2->OnUnhovered.AddDynamic(this, &USlotListWidget::Slot2UnHovered);
	Button2->OnPressed.AddDynamic(this, &USlotListWidget::Slot2Pressed);
	Button2->OnReleased.AddDynamic(this, &USlotListWidget::Slot2Released);

	UBorder* Icon2 = Cast<UBorder>(GetWidgetFromName(TEXT("Icon2")));
	SlotIconArray.Add(Icon2);

	UButton* Button3 = Cast<UButton>(GetWidgetFromName(TEXT("Slot3")));
	SlotArray.Add(Button3);
	Button3->OnHovered.AddDynamic(this, &USlotListWidget::Slot3Hovered);
	Button3->OnUnhovered.AddDynamic(this, &USlotListWidget::Slot3UnHovered);
	Button3->OnPressed.AddDynamic(this, &USlotListWidget::Slot3Pressed);
	Button3->OnReleased.AddDynamic(this, &USlotListWidget::Slot3Released);

	UBorder* Icon3 = Cast<UBorder>(GetWidgetFromName(TEXT("Icon3")));
	SlotIconArray.Add(Icon3);

	UButton* Button4 = Cast<UButton>(GetWidgetFromName(TEXT("Slot4")));
	SlotArray.Add(Button4);
	Button4->OnHovered.AddDynamic(this, &USlotListWidget::Slot4Hovered);
	Button4->OnUnhovered.AddDynamic(this, &USlotListWidget::Slot4UnHovered);
	Button4->OnPressed.AddDynamic(this, &USlotListWidget::Slot4Pressed);
	Button4->OnReleased.AddDynamic(this, &USlotListWidget::Slot4Released);

	UBorder* Icon4 = Cast<UBorder>(GetWidgetFromName(TEXT("Icon4")));
	SlotIconArray.Add(Icon4);

	//UCanvasPanel* Canvas = Cast<UCanvasPanel>(WidgetTree->FindWidget(TEXT("CanvasPanel")));

	//UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Canvas->Slot);
	//FVector2D	CanvasSize = CanvasSlot->GetSize();

	TArray<UButton*>	BtnArray;
	BtnArray.Add(Button1);
	BtnArray.Add(Button2);
	BtnArray.Add(Button3);
	BtnArray.Add(Button4);

	for (int32 i = 0; i < 4; ++i)
	{
		UCanvasPanelSlot* ButtonSlot = Cast<UCanvasPanelSlot>(BtnArray[i]->Slot);

		FVector2D Pos = ButtonSlot->GetPosition();
		FVector2D Size = ButtonSlot->GetSize();

		// Canvas의 크기를 구한다.
		//FAnchors Anchor = ButtonSlot->GetAnchors();

		//Pos.X += (CanvasSize.X * 0.5f);
		//Pos.Y += (CanvasSize.Y * 0.5f);
		Pos.X += 200.f;
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

	for (int i = 0; i < 4; ++i)
	{
		SkillInfoArray.Add(nullptr);
		m_SkillIconImageArray.Add(nullptr);
	}

	//CanvasSlot->SetPosition(FVector2D(MousePos.X, MousePos.Y));//
	//CanvasSlot->SetAlignment(FVector2D(0.5f, 0.5f));
	//CanvasSlot->SetSize(FVector2D(70.f, 70.f));

	SkillInfoWidget = Cast<USkillInfoWidget>(GetWidgetFromName(TEXT("UI_SkillInfo")));

	m_bMouseHovered = false;
	m_bMousePressed = false;
	m_bMousePressStart = false;
	m_bMouseDrag = false;
	m_DragSlotIndex = -1;
}

void USlotListWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (m_bMouseHovered)
	{
		FVector2D	PixelPosition, ViewportMin, ViewportMax;

		USlateBlueprintLibrary::LocalToViewport(GetWorld(), MyGeometry, FVector2D(0.f, 0.f), PixelPosition, ViewportMin);
		USlateBlueprintLibrary::LocalToViewport(GetWorld(), MyGeometry, MyGeometry.GetLocalSize(), PixelPosition, ViewportMax);

		FVector2D	MousePos = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());

		MousePos -= ViewportMin;

		UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(SkillInfoWidget->Slot);

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

void USlotListWidget::ComputeWorldPos(const FVector2D& Pos, const FVector2D& Ratio)
{
	for (int32 i = 0; i < SlotRectArray.Num(); ++i)
	{
		SlotRectArray[i].Pos += Pos;

		LOG(TEXT("%d Slot Pos : %s, size : %s"), i, *SlotRectArray[i].Pos.ToString(), *SlotRectArray[i].Size.ToString());
	}
}

int32 USlotListWidget::CollisionSlot(const FVector2D& MousePos)
{
	for (int32 i = 0; i < SlotRectArray.Num(); ++i)
	{
		if (SlotRectArray[i].Pos.X <= MousePos.X && MousePos.X <= SlotRectArray[i].Pos.X + SlotRectArray[i].Size.X &&
			SlotRectArray[i].Pos.Y <= MousePos.Y && MousePos.Y <= SlotRectArray[i].Pos.Y + SlotRectArray[i].Size.Y)
		{

			PrintViewport(5.f, FColor::Blue, FString::Printf(TEXT("SkillSlot %d"), i));
			return i;
		}
	}

	return -1;
}

void USlotListWidget::Slot1Hovered()
{
	if (!SkillInfoArray[0])
		return;

	SkillInfoWidget->SetVisibility(ESlateVisibility::Visible);

	SkillInfoWidget->SetSkillInfo(SkillInfoArray[0]);
	FString  Level = FString::Printf(TEXT("%d"), 1);
	SkillInfoWidget->SetSkillLevel(Level);

	m_bMouseHovered = true;
}

void USlotListWidget::Slot2Hovered()
{
	if (!SkillInfoArray[1])
		return;

	SkillInfoWidget->SetVisibility(ESlateVisibility::Visible);

	SkillInfoWidget->SetSkillInfo(SkillInfoArray[1]);
	FString  Level = FString::Printf(TEXT("%d"), 1);
	SkillInfoWidget->SetSkillLevel(Level);

	m_bMouseHovered = true;
}

void USlotListWidget::Slot3Hovered()
{
	if (!SkillInfoArray[2])
		return;

	SkillInfoWidget->SetVisibility(ESlateVisibility::Visible);

	SkillInfoWidget->SetSkillInfo(SkillInfoArray[2]);
	FString  Level = FString::Printf(TEXT("%d"), 1);
	SkillInfoWidget->SetSkillLevel(Level);

	m_bMouseHovered = true;
}

void USlotListWidget::Slot4Hovered()
{
	if (!SkillInfoArray[3])
		return;

	SkillInfoWidget->SetVisibility(ESlateVisibility::Visible);

	SkillInfoWidget->SetSkillInfo(SkillInfoArray[3]);
	FString  Level = FString::Printf(TEXT("%d"), 1);
	SkillInfoWidget->SetSkillLevel(Level);

	m_bMouseHovered = true;
}

void USlotListWidget::Slot1UnHovered()
{
	if (!SkillInfoWidget)
		return;

	m_bMouseHovered = false;

	if(SkillInfoWidget->GetVisibility() == ESlateVisibility::Visible)
		SkillInfoWidget->SetVisibility(ESlateVisibility::Collapsed);
}

void USlotListWidget::Slot2UnHovered()
{
	if (!SkillInfoWidget)
		return;

	m_bMouseHovered = false;

	if (SkillInfoWidget->GetVisibility() == ESlateVisibility::Visible)
		SkillInfoWidget->SetVisibility(ESlateVisibility::Collapsed);
}

void USlotListWidget::Slot3UnHovered()
{
	if (!SkillInfoWidget)
		return;

	m_bMouseHovered = false;

	if (SkillInfoWidget->GetVisibility() == ESlateVisibility::Visible)
		SkillInfoWidget->SetVisibility(ESlateVisibility::Collapsed);
}

void USlotListWidget::Slot4UnHovered()
{
	if (!SkillInfoWidget)
		return;

	m_bMouseHovered = false;

	if (SkillInfoWidget->GetVisibility() == ESlateVisibility::Visible)
		SkillInfoWidget->SetVisibility(ESlateVisibility::Collapsed);
}

void USlotListWidget::Slot1Pressed()
{
	PrintViewport(1.f, FColor::Red, TEXT("SkillWidget Pressed"));

	if (SkillInfoArray[0])
	{
		m_bMousePressed = true;
		m_bMousePressStart = true;
		m_DragSlotIndex = 0;

		SkillInfoWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void USlotListWidget::Slot2Pressed()
{
	PrintViewport(1.f, FColor::Red, TEXT("SkillWidget Pressed"));

	if (SkillInfoArray[1])
	{
		m_bMousePressed = true;
		m_bMousePressStart = true;
		m_DragSlotIndex = 1;

		SkillInfoWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void USlotListWidget::Slot3Pressed()
{
	PrintViewport(1.f, FColor::Red, TEXT("SkillWidget Pressed"));

	if (SkillInfoArray[2])
	{
		m_bMousePressed = true;
		m_bMousePressStart = true;
		m_DragSlotIndex = 2;

		SkillInfoWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void USlotListWidget::Slot4Pressed()
{
	PrintViewport(1.f, FColor::Red, TEXT("SkillWidget Pressed"));

	if (SkillInfoArray[3])
	{
		m_bMousePressed = true;
		m_bMousePressStart = true;
		m_DragSlotIndex = 3;

		SkillInfoWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void USlotListWidget::Slot1Released()
{
	m_bMousePressed = false;
	m_bMouseDrag = false;
	m_DragSlotIndex = -1;

	if (m_bMouseHovered)
		SkillInfoWidget->SetVisibility(ESlateVisibility::Visible);
}

void USlotListWidget::Slot2Released()
{
	m_bMousePressed = false;
	m_bMouseDrag = false;
	m_DragSlotIndex = -1;

	if (m_bMouseHovered)
		SkillInfoWidget->SetVisibility(ESlateVisibility::Visible);
}

void USlotListWidget::Slot3Released()
{
	m_bMousePressed = false;
	m_bMouseDrag = false;
	m_DragSlotIndex = -1;

	if (m_bMouseHovered)
		SkillInfoWidget->SetVisibility(ESlateVisibility::Visible);
}

void USlotListWidget::Slot4Released()
{
	m_bMousePressed = false;
	m_bMouseDrag = false;
	m_DragSlotIndex = -1;

	if (m_bMouseHovered)
		SkillInfoWidget->SetVisibility(ESlateVisibility::Visible);
}
