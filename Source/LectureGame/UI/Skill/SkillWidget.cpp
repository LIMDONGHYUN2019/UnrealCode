// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillWidget.h"
#include "../GameWidget.h"
#include "../../GameMode/LectureGameGameModeBase.h"
#include "../../Player/PlayerCharacter.h"
#include "Components/CanvasPanelSlot.h"
#include "../../NetworkManager.h"
#include "../../GameServerManager.h"
#include "../../PacketSession.h"
#include "../../NetStream.h"


void USkillWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CloseButton = Cast<UButton>(GetWidgetFromName(TEXT("CloseButton")));
	TitlebarButton = Cast<UButton>(GetWidgetFromName(TEXT("SkillTile")));

	CloseButton->OnClicked.AddDynamic(this, &USkillWidget::CloseSkill);
	TitlebarButton->OnPressed.AddDynamic(this, &USkillWidget::TilePressed);
	TitlebarButton->OnReleased.AddDynamic(this, &USkillWidget::TileReleased);

	UButton* Button = Cast<UButton>(GetWidgetFromName(TEXT("Icon1_1")));
	SkillButton.Add(Button);
	Button->OnClicked.AddDynamic(this, &USkillWidget::SkillIcon1_1);
	Button->OnHovered.AddDynamic(this, &USkillWidget::SkillIcon1_1_Hovered);
	Button->OnUnhovered.AddDynamic(this, &USkillWidget::SkillIcon1_1_UnHovered);
	Button->OnPressed.AddDynamic(this, &USkillWidget::SkillIcon1_1_Pressed);
	Button->OnReleased.AddDynamic(this, &USkillWidget::SkillIcon1_1_Released);

	Button = Cast<UButton>(GetWidgetFromName(TEXT("Icon1_2")));
	SkillButton.Add(Button);
	Button->OnClicked.AddDynamic(this, &USkillWidget::SkillIcon1_2);
	Button->OnHovered.AddDynamic(this, &USkillWidget::SkillIcon1_2_Hovered);
	Button->OnUnhovered.AddDynamic(this, &USkillWidget::SkillIcon1_2_UnHovered);
	Button->OnPressed.AddDynamic(this, &USkillWidget::SkillIcon1_2_Pressed);
	Button->OnReleased.AddDynamic(this, &USkillWidget::SkillIcon1_2_Released);

	Button = Cast<UButton>(GetWidgetFromName(TEXT("Icon1_3")));
	SkillButton.Add(Button);
	Button->OnClicked.AddDynamic(this, &USkillWidget::SkillIcon1_3);
	Button->OnHovered.AddDynamic(this, &USkillWidget::SkillIcon1_3_Hovered);
	Button->OnUnhovered.AddDynamic(this, &USkillWidget::SkillIcon1_3_UnHovered);
	Button->OnPressed.AddDynamic(this, &USkillWidget::SkillIcon1_3_Pressed);
	Button->OnReleased.AddDynamic(this, &USkillWidget::SkillIcon1_3_Released);

	Button = Cast<UButton>(GetWidgetFromName(TEXT("Icon2_1")));
	SkillButton.Add(Button);
	Button->OnClicked.AddDynamic(this, &USkillWidget::SkillIcon2_1);
	Button->OnHovered.AddDynamic(this, &USkillWidget::SkillIcon2_1_Hovered);
	Button->OnUnhovered.AddDynamic(this, &USkillWidget::SkillIcon2_1_UnHovered);
	Button->OnPressed.AddDynamic(this, &USkillWidget::SkillIcon2_1_Pressed);
	Button->OnReleased.AddDynamic(this, &USkillWidget::SkillIcon2_1_Released);

	Button = Cast<UButton>(GetWidgetFromName(TEXT("Icon2_2")));
	SkillButton.Add(Button);
	Button->OnClicked.AddDynamic(this, &USkillWidget::SkillIcon2_2);
	Button->OnHovered.AddDynamic(this, &USkillWidget::SkillIcon2_2_Hovered);
	Button->OnUnhovered.AddDynamic(this, &USkillWidget::SkillIcon2_2_UnHovered);
	Button->OnPressed.AddDynamic(this, &USkillWidget::SkillIcon2_2_Pressed);
	Button->OnReleased.AddDynamic(this, &USkillWidget::SkillIcon2_2_Released);

	Button = Cast<UButton>(GetWidgetFromName(TEXT("Icon2_3")));
	SkillButton.Add(Button);
	Button->OnClicked.AddDynamic(this, &USkillWidget::SkillIcon2_3);
	Button->OnHovered.AddDynamic(this, &USkillWidget::SkillIcon2_3_Hovered);
	Button->OnUnhovered.AddDynamic(this, &USkillWidget::SkillIcon2_3_UnHovered);
	Button->OnPressed.AddDynamic(this, &USkillWidget::SkillIcon2_3_Pressed);
	Button->OnReleased.AddDynamic(this, &USkillWidget::SkillIcon2_3_Released);

	Button = Cast<UButton>(GetWidgetFromName(TEXT("Icon2_4")));
	SkillButton.Add(Button);
	Button->OnClicked.AddDynamic(this, &USkillWidget::SkillIcon2_4);
	Button->OnHovered.AddDynamic(this, &USkillWidget::SkillIcon2_4_Hovered);
	Button->OnUnhovered.AddDynamic(this, &USkillWidget::SkillIcon2_4_UnHovered);
	Button->OnPressed.AddDynamic(this, &USkillWidget::SkillIcon2_4_Pressed);
	Button->OnReleased.AddDynamic(this, &USkillWidget::SkillIcon2_4_Released);

	Button = Cast<UButton>(GetWidgetFromName(TEXT("Icon3_1")));
	SkillButton.Add(Button);
	Button->OnClicked.AddDynamic(this, &USkillWidget::SkillIcon3_1);
	Button->OnHovered.AddDynamic(this, &USkillWidget::SkillIcon3_1_Hovered);
	Button->OnUnhovered.AddDynamic(this, &USkillWidget::SkillIcon3_1_UnHovered);
	Button->OnPressed.AddDynamic(this, &USkillWidget::SkillIcon3_1_Pressed);
	Button->OnReleased.AddDynamic(this, &USkillWidget::SkillIcon3_1_Released);

	Button = Cast<UButton>(GetWidgetFromName(TEXT("Icon3_2")));
	SkillButton.Add(Button);
	Button->OnClicked.AddDynamic(this, &USkillWidget::SkillIcon3_2);
	Button->OnHovered.AddDynamic(this, &USkillWidget::SkillIcon3_2_Hovered);
	Button->OnUnhovered.AddDynamic(this, &USkillWidget::SkillIcon3_2_UnHovered);
	Button->OnPressed.AddDynamic(this, &USkillWidget::SkillIcon3_2_Pressed);
	Button->OnReleased.AddDynamic(this, &USkillWidget::SkillIcon3_2_Released);

	Button = Cast<UButton>(GetWidgetFromName(TEXT("Icon3_3")));
	SkillButton.Add(Button);
	Button->OnClicked.AddDynamic(this, &USkillWidget::SkillIcon3_3);
	Button->OnHovered.AddDynamic(this, &USkillWidget::SkillIcon3_3_Hovered);
	Button->OnUnhovered.AddDynamic(this, &USkillWidget::SkillIcon3_3_UnHovered);
	Button->OnPressed.AddDynamic(this, &USkillWidget::SkillIcon3_3_Pressed);
	Button->OnReleased.AddDynamic(this, &USkillWidget::SkillIcon3_3_Released);

	for (int32 i = 0; i < SkillButton.Num(); ++i)
	{
		m_SkillIconImageArray.Add(nullptr);
	}

	UTextBlock* Text = Cast<UTextBlock>(GetWidgetFromName(TEXT("Level1_1")));
	SkillLevelText.Add(Text);
	Text = Cast<UTextBlock>(GetWidgetFromName(TEXT("Level1_2")));
	SkillLevelText.Add(Text);
	Text = Cast<UTextBlock>(GetWidgetFromName(TEXT("Level1_3")));
	SkillLevelText.Add(Text);

	Text = Cast<UTextBlock>(GetWidgetFromName(TEXT("Level2_1")));
	SkillLevelText.Add(Text);
	Text = Cast<UTextBlock>(GetWidgetFromName(TEXT("Level2_2")));
	SkillLevelText.Add(Text);
	Text = Cast<UTextBlock>(GetWidgetFromName(TEXT("Level2_3")));
	SkillLevelText.Add(Text);
	Text = Cast<UTextBlock>(GetWidgetFromName(TEXT("Level2_4")));
	SkillLevelText.Add(Text);

	Text = Cast<UTextBlock>(GetWidgetFromName(TEXT("Level3_1")));
	SkillLevelText.Add(Text);
	Text = Cast<UTextBlock>(GetWidgetFromName(TEXT("Level3_2")));
	SkillLevelText.Add(Text);
	Text = Cast<UTextBlock>(GetWidgetFromName(TEXT("Level3_3")));
	SkillLevelText.Add(Text);

	PointText = Cast<UTextBlock>(GetWidgetFromName(TEXT("PointText")));

	SkillInfoWidget = Cast<USkillInfoWidget>(GetWidgetFromName(TEXT("UI_SkillInfo")));

	m_bMouseHovered = false;
	m_bMousePressed = false;
	m_bMousePressStart = false;
	m_bMouseDrag = false;

	m_bSkillIconDrag = false;
	m_bTitlebarDrag = false;

	m_DragSkillIndex = -1;

}

void USkillWidget::ChangeIcon(int32 Index, UTexture2D* Image)
{
	m_SkillIconImageArray[Index] = Image;

	FButtonStyle	style;
	FSlateBrush		brush;
	brush.ImageSize = FVector2D(132.f, 129.f);
	brush.Margin = FMargin();
	brush.TintColor = FSlateColor(FLinearColor(0.8f, 0.8f, 0.8f, 1.f));
	brush.DrawAs = ESlateBrushDrawType::Image;
	brush.Tiling = ESlateBrushTileType::NoTile;
	brush.SetResourceObject(Image);
	style.SetNormal(brush);

	brush.ImageSize = FVector2D(132.f, 129.f);
	brush.Margin = FMargin();
	brush.TintColor = FSlateColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
	brush.DrawAs = ESlateBrushDrawType::Image;
	brush.Tiling = ESlateBrushTileType::NoTile;
	brush.SetResourceObject(Image);
	style.SetHovered(brush);

	brush.ImageSize = FVector2D(132.f, 129.f);
	brush.Margin = FMargin();
	brush.TintColor = FSlateColor(FLinearColor(0.6f, 0.6f, 0.6f, 1.f));
	brush.DrawAs = ESlateBrushDrawType::Image;
	brush.Tiling = ESlateBrushTileType::NoTile;
	brush.SetResourceObject(Image);
	style.SetPressed(brush);

	brush.ImageSize = FVector2D(132.f, 129.f);
	brush.Margin = FMargin();
	brush.TintColor = FSlateColor(FLinearColor(0.2f, 0.2f, 0.2f, 1.f));
	brush.DrawAs = ESlateBrushDrawType::Image;   
	brush.Tiling = ESlateBrushTileType::NoTile;
	brush.SetResourceObject(Image);
	style.SetDisabled(brush);

	SkillButton[Index]->SetStyle(style);

	//brush.ImageType = ESlateBrushImageType::Linear;
}

void USkillWidget::ChangeSkillLevel(int32 Index, int32 Level)
{
	SkillLevelText[Index]->SetText(FText::FromString(FString::Printf(TEXT("%d"), Level)));

	if (Level > 0)
	{
		FString Str = SkillLevelText[Index]->GetText().ToString();
		SkillInfoWidget->SetSkillLevel(Str);

		SkillButton[Index]->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
	}
}

void USkillWidget::SetSkillPoint(int32 Point)
{
	if (Point < 0 || Point > 100)
		return;

	m_SkillPoint = Point;
	PointText->SetText(FText::FromString(FString::Printf(TEXT("%d"), Point)));
}

void USkillWidget::SkillPointUp(int32 Point)
{
	if (Point < 0 || Point > 100)
		return;

	m_SkillPoint += Point;
	PointText->SetText(FText::FromString(FString::Printf(TEXT("%d"), m_SkillPoint)));
}

void USkillWidget::SetSkillInfoWidget(int32 Index)
{
	//SkillInfoWidget->SetSkillName(UTF8_TO_TCHAR(m_SkillInfoArray[Index]->strName));
	//SkillInfoWidget->SetSkillDesc(UTF8_TO_TCHAR(m_SkillInfoArray[Index]->strDesc));

	//FString Level = SkillLevelText[Index]->GetText().ToString();
	//SkillInfoWidget->SetSkillLevel(Level);

	//SkillInfoWidget->SetSkillType(m_SkillInfoArray[Index]->eType);
	//SkillInfoWidget->SetSkillProperty(m_SkillInfoArray[Index]->ePropt);
	//SkillInfoWidget->SetSkillCooldown(m_SkillInfoArray[Index]->fCooldown);
	//SkillInfoWidget->SetSkillLevelMax(m_SkillInfoArray[Index]->iSkillLevelMax);
	//SkillInfoWidget->SetSkillRequiredLevel(m_SkillInfoArray[Index]->iRequiredLevel);

	//SkillInfoWidget->OptionClear();
	//for (int32 Count = 0; Count < m_SkillInfoArray[Index]->vecOption.Num(); ++Count)
	//{
	//	SkillInfoWidget->SetSkillOption(m_SkillInfoArray[Index]->vecOption[Count].SkillOptionType, m_SkillInfoArray[Index]->vecOption[Count].SkillOptionData, Count);
	//}

	SkillInfoWidget->SetSkillInfo(m_SkillInfoArray[Index]);
}

void USkillWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
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

		CanvasSlot->SetPosition(FVector2D(MousePos.X + 3.f, MousePos.Y + 3.f));
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
			//LOG(TEXT("PressStartPosX = %.5f PressStartPosY = %.5f"), PressStartPos.X, PressStartPos.Y);
			PressStartPos -= ViewportMin;

			//LOG(TEXT("ViewportMinX = %.5f ViewportMinY = %.5f"), ViewportMin.X, ViewportMin.Y);
			m_bMousePressStart = false;

			m_Distance = PressStartPos; // 마우스클릭과 Min 거리차이

			//LOG(TEXT("m_DistanceX = %.5f m_DistanceY = %.5f"), m_Distance.X, m_Distance.Y);
		}

		else if(!m_bMouseDrag)
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

void USkillWidget::CloseSkill()
{
	SetVisibility(ESlateVisibility::Collapsed);
}

void USkillWidget::TilePressed()
{
	m_bMousePressed = true;
	m_bMousePressStart = true;
	m_bTitlebarDrag = true;
}

void USkillWidget::TileReleased()
{
	m_bMousePressed = false;
	m_bMousePressStart = false;
	m_bTitlebarDrag = false;
}

void USkillWidget::SkillIcon1_1()
{
	if (m_bMouseDrag)
		return;

	uint8	Packet[PACKET_LENGTH] = {};
	NetStream	stream;
	stream.SetBuffer(Packet);

	stream.Write(&m_SkillInfoArray[0]->iSkillNumber, 4);

	PacketSession* Session = GameServerManager::GetInst()->GetSession();
	Session->Write(GP_SkillUp, stream.GetLength(), Packet);
}

void USkillWidget::SkillIcon1_2()
{
	if (m_bMouseDrag)
		return;

	uint8	Packet[PACKET_LENGTH] = {};
	NetStream	stream;
	stream.SetBuffer(Packet);

	stream.Write(&m_SkillInfoArray[1]->iSkillNumber, 4);

	PacketSession* Session = GameServerManager::GetInst()->GetSession();
	Session->Write(GP_SkillUp, stream.GetLength(), Packet);
}

void USkillWidget::SkillIcon1_3()
{
	if (m_bMouseDrag)
		return;

	uint8	Packet[PACKET_LENGTH] = {};
	NetStream	stream;
	stream.SetBuffer(Packet);

	stream.Write(&m_SkillInfoArray[2]->iSkillNumber, 4);

	PacketSession* Session = GameServerManager::GetInst()->GetSession();
	Session->Write(GP_SkillUp, stream.GetLength(), Packet);
}

void USkillWidget::SkillIcon2_1()
{
	if (m_bMouseDrag)
		return;

	uint8	Packet[PACKET_LENGTH] = {};
	NetStream	stream;
	stream.SetBuffer(Packet);

	stream.Write(&m_SkillInfoArray[3]->iSkillNumber, 4);

	PacketSession* Session = GameServerManager::GetInst()->GetSession();
	Session->Write(GP_SkillUp, stream.GetLength(), Packet);
}

void USkillWidget::SkillIcon2_2()
{
	if (m_bMouseDrag)
		return;

	uint8	Packet[PACKET_LENGTH] = {};
	NetStream	stream;
	stream.SetBuffer(Packet);

	stream.Write(&m_SkillInfoArray[4]->iSkillNumber, 4);

	PacketSession* Session = GameServerManager::GetInst()->GetSession();
	Session->Write(GP_SkillUp, stream.GetLength(), Packet);
}

void USkillWidget::SkillIcon2_3()
{
	if (m_bMouseDrag)
		return;

	uint8	Packet[PACKET_LENGTH] = {};
	NetStream	stream;
	stream.SetBuffer(Packet);

	stream.Write(&m_SkillInfoArray[5]->iSkillNumber, 4);

	PacketSession* Session = GameServerManager::GetInst()->GetSession();
	Session->Write(GP_SkillUp, stream.GetLength(), Packet);
}

void USkillWidget::SkillIcon2_4()
{
	if (m_bMouseDrag)
		return;

	uint8	Packet[PACKET_LENGTH] = {};
	NetStream	stream;
	stream.SetBuffer(Packet);

	stream.Write(&m_SkillInfoArray[6]->iSkillNumber, 4);

	PacketSession* Session = GameServerManager::GetInst()->GetSession();
	Session->Write(GP_SkillUp, stream.GetLength(), Packet);
}

void USkillWidget::SkillIcon3_1()
{
	if (m_bMouseDrag)
		return;

	uint8	Packet[PACKET_LENGTH] = {};
	NetStream	stream;
	stream.SetBuffer(Packet);

	stream.Write(&m_SkillInfoArray[7]->iSkillNumber, 4);

	PacketSession* Session = GameServerManager::GetInst()->GetSession();
	Session->Write(GP_SkillUp, stream.GetLength(), Packet);
}

void USkillWidget::SkillIcon3_2()
{
	if (m_bMouseDrag)
		return;

	uint8	Packet[PACKET_LENGTH] = {};
	NetStream	stream;
	stream.SetBuffer(Packet);

	stream.Write(&m_SkillInfoArray[8]->iSkillNumber, 4);

	PacketSession* Session = GameServerManager::GetInst()->GetSession();
	Session->Write(GP_SkillUp, stream.GetLength(), Packet);
}

void USkillWidget::SkillIcon3_3()
{
	if (m_bMouseDrag)
		return;

	uint8	Packet[PACKET_LENGTH] = {};
	NetStream	stream;
	stream.SetBuffer(Packet);

	stream.Write(&m_SkillInfoArray[9]->iSkillNumber, 4);

	PacketSession* Session = GameServerManager::GetInst()->GetSession();
	Session->Write(GP_SkillUp, stream.GetLength(), Packet);
}

void USkillWidget::SkillIcon1_1_Hovered()
{
	if (m_bMouseDrag)
		return;

	m_bMouseHovered = true;

	SetSkillInfoWidget(0);

	SkillInfoWidget->SetVisibility(ESlateVisibility::Visible);
}

void USkillWidget::SkillIcon1_2_Hovered()
{
	if (m_bMouseDrag)
		return;

	m_bMouseHovered = true;

	SetSkillInfoWidget(1);

	SkillInfoWidget->SetVisibility(ESlateVisibility::Visible);
}

void USkillWidget::SkillIcon1_3_Hovered()
{
	if (m_bMouseDrag)
		return;

	m_bMouseHovered = true;

	SetSkillInfoWidget(2);

	SkillInfoWidget->SetVisibility(ESlateVisibility::Visible);
}

void USkillWidget::SkillIcon2_1_Hovered()
{
	if (m_bMouseDrag)
		return;

	m_bMouseHovered = true;

	SetSkillInfoWidget(3);

	SkillInfoWidget->SetVisibility(ESlateVisibility::Visible);
}

void USkillWidget::SkillIcon2_2_Hovered()
{
	if (m_bMouseDrag)
		return;

	m_bMouseHovered = true;

	SetSkillInfoWidget(4);

	SkillInfoWidget->SetVisibility(ESlateVisibility::Visible);
}

void USkillWidget::SkillIcon2_3_Hovered()
{
	if (m_bMouseDrag)
		return;

	m_bMouseHovered = true;

	SetSkillInfoWidget(5);

	SkillInfoWidget->SetVisibility(ESlateVisibility::Visible);
}

void USkillWidget::SkillIcon2_4_Hovered()
{
	if (m_bMouseDrag)
		return;

	m_bMouseHovered = true;

	SetSkillInfoWidget(6);

	SkillInfoWidget->SetVisibility(ESlateVisibility::Visible);
}

void USkillWidget::SkillIcon3_1_Hovered()
{
	if (m_bMouseDrag)
		return;

	m_bMouseHovered = true;

	SetSkillInfoWidget(7);

	SkillInfoWidget->SetVisibility(ESlateVisibility::Visible);
}

void USkillWidget::SkillIcon3_2_Hovered()
{
	if (m_bMouseDrag)
		return;

	m_bMouseHovered = true;

	SetSkillInfoWidget(8);

	SkillInfoWidget->SetVisibility(ESlateVisibility::Visible);
}

void USkillWidget::SkillIcon3_3_Hovered()
{
	if (m_bMouseDrag)
		return;

	m_bMouseHovered = true;

	SetSkillInfoWidget(9);

	SkillInfoWidget->SetVisibility(ESlateVisibility::Visible);
}

void USkillWidget::SkillIcon1_1_UnHovered()
{
	m_bMouseHovered = false;

	SkillInfoWidget->SetVisibility(ESlateVisibility::Collapsed);
}

void USkillWidget::SkillIcon1_2_UnHovered()
{
	m_bMouseHovered = false;

	SkillInfoWidget->SetVisibility(ESlateVisibility::Collapsed);
}

void USkillWidget::SkillIcon1_3_UnHovered()
{
	m_bMouseHovered = false;

	SkillInfoWidget->SetVisibility(ESlateVisibility::Collapsed);
}

void USkillWidget::SkillIcon2_1_UnHovered()
{
	m_bMouseHovered = false;
	SkillInfoWidget->SetVisibility(ESlateVisibility::Collapsed);
}

void USkillWidget::SkillIcon2_2_UnHovered()
{
	m_bMouseHovered = false;

	SkillInfoWidget->SetVisibility(ESlateVisibility::Collapsed);
}

void USkillWidget::SkillIcon2_3_UnHovered()
{
	m_bMouseHovered = false;

	SkillInfoWidget->SetVisibility(ESlateVisibility::Collapsed);
}

void USkillWidget::SkillIcon2_4_UnHovered()
{
	m_bMouseHovered = false;

	SkillInfoWidget->SetVisibility(ESlateVisibility::Collapsed);
}

void USkillWidget::SkillIcon3_1_UnHovered()
{
	m_bMouseHovered = false;

	SkillInfoWidget->SetVisibility(ESlateVisibility::Collapsed);
}

void USkillWidget::SkillIcon3_2_UnHovered()
{
	m_bMouseHovered = false;

	SkillInfoWidget->SetVisibility(ESlateVisibility::Collapsed);
}

void USkillWidget::SkillIcon3_3_UnHovered()
{
	m_bMouseHovered = false;

	SkillInfoWidget->SetVisibility(ESlateVisibility::Collapsed);
}

void USkillWidget::SkillIcon1_1_Pressed()
{
	if (m_SkillInfoArray[0]->eType == ESkillType::Passive)
		return;

	FString Str = SkillLevelText[0]->GetText().ToString();
	int Level = FCString::Atoi(*Str);
	if (!Level == 0)
	{
		m_bMousePressed = true;
		m_bMousePressStart = true;
		m_bSkillIconDrag = true;
		m_DragSkillIndex = 0;

		SkillInfoWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void USkillWidget::SkillIcon1_2_Pressed()
{
	if (m_SkillInfoArray[1]->eType == ESkillType::Passive)
		return;

	FString Str = SkillLevelText[1]->GetText().ToString();
	int Level = FCString::Atoi(*Str);
	if (!Level == 0)
	{
		m_bMousePressed = true;
		m_bMousePressStart = true;
		m_bSkillIconDrag = true;
		m_DragSkillIndex = 1;

		SkillInfoWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void USkillWidget::SkillIcon1_3_Pressed()
{
	if (m_SkillInfoArray[2]->eType == ESkillType::Passive)
		return;

	FString Str = SkillLevelText[2]->GetText().ToString();
	int Level = FCString::Atoi(*Str);
	if (!Level == 0)
	{
		m_bMousePressed = true;
		m_bMousePressStart = true;
		m_bSkillIconDrag = true;
		m_DragSkillIndex = 2;

		SkillInfoWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void USkillWidget::SkillIcon2_1_Pressed()
{
	if (m_SkillInfoArray[3]->eType == ESkillType::Passive)
		return;

	FString Str = SkillLevelText[3]->GetText().ToString();
	int Level = FCString::Atoi(*Str);
	if (!Level == 0)
	{
		m_bMousePressed = true;
		m_bMousePressStart = true;
		m_bSkillIconDrag = true;
		m_DragSkillIndex = 3;

		SkillInfoWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void USkillWidget::SkillIcon2_2_Pressed()
{
	if (m_SkillInfoArray[4]->eType == ESkillType::Passive)
		return;

	FString Str = SkillLevelText[4]->GetText().ToString();
	int Level = FCString::Atoi(*Str);
	if (!Level == 0)
	{
		m_bMousePressed = true;
		m_bMousePressStart = true;
		m_bSkillIconDrag = true;
		m_DragSkillIndex = 4;

		SkillInfoWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void USkillWidget::SkillIcon2_3_Pressed()
{
	if (m_SkillInfoArray[5]->eType == ESkillType::Passive)
		return;

	FString Str = SkillLevelText[5]->GetText().ToString();
	int Level = FCString::Atoi(*Str);
	if (!Level == 0)
	{
		m_bMousePressed = true;
		m_bMousePressStart = true;
		m_bSkillIconDrag = true;
		m_DragSkillIndex = 5;

		SkillInfoWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void USkillWidget::SkillIcon2_4_Pressed()
{
	if (m_SkillInfoArray[6]->eType == ESkillType::Passive)
		return;

	FString Str = SkillLevelText[6]->GetText().ToString();
	int Level = FCString::Atoi(*Str);
	if (!Level == 0)
	{
		m_bMousePressed = true;
		m_bMousePressStart = true;
		m_bSkillIconDrag = true;
		m_DragSkillIndex = 6;

		SkillInfoWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void USkillWidget::SkillIcon3_1_Pressed()
{
	if (m_SkillInfoArray[7]->eType == ESkillType::Passive)
		return;

	FString Str = SkillLevelText[7]->GetText().ToString();
	int Level = FCString::Atoi(*Str);
	if (!Level == 0)
	{
		m_bMousePressed = true;
		m_bMousePressStart = true;
		m_bSkillIconDrag = true;
		m_DragSkillIndex = 7;

		SkillInfoWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void USkillWidget::SkillIcon3_2_Pressed()
{
	if (m_SkillInfoArray[8]->eType == ESkillType::Passive)
		return;

	FString Str = SkillLevelText[8]->GetText().ToString();
	int Level = FCString::Atoi(*Str);
	if (!Level == 0)
	{
		m_bMousePressed = true;
		m_bMousePressStart = true;
		m_bSkillIconDrag = true;
		m_DragSkillIndex = 8;

		SkillInfoWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void USkillWidget::SkillIcon3_3_Pressed()
{
	if (m_SkillInfoArray[9]->eType == ESkillType::Passive)
		return;

	FString Str = SkillLevelText[9]->GetText().ToString();
	int Level = FCString::Atoi(*Str);
	if (!Level == 0)
	{
		m_bMousePressed = true;
		m_bMousePressStart = true;
		m_bSkillIconDrag = true;
		m_DragSkillIndex = 9;

		SkillInfoWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void USkillWidget::SkillIcon1_1_Released()
{
	m_bMousePressed = false;
	m_bMouseDrag = false;
	m_bSkillIconDrag = false;
	m_DragSkillIndex = -1;

	if (m_bMouseHovered)
		SkillInfoWidget->SetVisibility(ESlateVisibility::Visible);
}

void USkillWidget::SkillIcon1_2_Released()
{
	m_bMousePressed = false;
	m_bMouseDrag = false;
	m_bSkillIconDrag = false;
	m_DragSkillIndex = -1;

	if (m_bMouseHovered)
		SkillInfoWidget->SetVisibility(ESlateVisibility::Visible);
}

void USkillWidget::SkillIcon1_3_Released()
{
	m_bMousePressed = false;
	m_bMouseDrag = false;
	m_bSkillIconDrag = false;
	m_DragSkillIndex = -1;

	if (m_bMouseHovered)
		SkillInfoWidget->SetVisibility(ESlateVisibility::Visible);
}

void USkillWidget::SkillIcon2_1_Released()
{
	m_bMousePressed = false;
	m_bMouseDrag = false;
	m_bSkillIconDrag = false;
	m_DragSkillIndex = -1;

	if (m_bMouseHovered)
		SkillInfoWidget->SetVisibility(ESlateVisibility::Visible);
}

void USkillWidget::SkillIcon2_2_Released()
{
	m_bMousePressed = false;
	m_bMouseDrag = false;
	m_bSkillIconDrag = false;
	m_DragSkillIndex = -1;

	if (m_bMouseHovered)
		SkillInfoWidget->SetVisibility(ESlateVisibility::Visible);
}

void USkillWidget::SkillIcon2_3_Released()
{
	m_bMousePressed = false;
	m_bMouseDrag = false;
	m_bSkillIconDrag = false;
	m_DragSkillIndex = -1;

	if (m_bMouseHovered)
		SkillInfoWidget->SetVisibility(ESlateVisibility::Visible);
}

void USkillWidget::SkillIcon2_4_Released()
{
	m_bMousePressed = false;
	m_bMouseDrag = false;
	m_bSkillIconDrag = false;
	m_DragSkillIndex = -1;

	if (m_bMouseHovered)
		SkillInfoWidget->SetVisibility(ESlateVisibility::Visible);
}

void USkillWidget::SkillIcon3_1_Released()
{
	m_bMousePressed = false;
	m_bMouseDrag = false;
	m_bSkillIconDrag = false;
	m_DragSkillIndex = -1;

	if (m_bMouseHovered)
		SkillInfoWidget->SetVisibility(ESlateVisibility::Visible);
}

void USkillWidget::SkillIcon3_2_Released()
{
	m_bMousePressed = false;
	m_bMouseDrag = false;
	m_bSkillIconDrag = false;
	m_DragSkillIndex = -1;

	if (m_bMouseHovered)
		SkillInfoWidget->SetVisibility(ESlateVisibility::Visible);
}

void USkillWidget::SkillIcon3_3_Released()
{
	m_bMousePressed = false;
	m_bMouseDrag = false;
	m_bSkillIconDrag = false;
	m_DragSkillIndex = -1;

	if (m_bMouseHovered)
		SkillInfoWidget->SetVisibility(ESlateVisibility::Visible);
}
