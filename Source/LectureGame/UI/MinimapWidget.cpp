// Fill out your copyright notice in the Description page of Project Settings.


#include "MinimapWidget.h"

void UMinimapWidget::NativeConstruct()
{
	Super::NativeConstruct();

	//IconImage = Cast<UImage>(GetWidgetFromName(TEXT("Icon")));

	m_CanvasMonster = Cast<UCanvasPanel>(WidgetTree->FindWidget(TEXT("CanvasMonster")));

	/*UImage* Image = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass());

	Image->SetRenderTranslation(FVector2D(100.f, 50.f));

	m_CanvasMonster->AddChild(Image);*/

	m_MonsterCount = 0;
}

void UMinimapWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UMinimapWidget::ClearMonster()
{
	for (int32 i = 0; i < m_MonsterCount; ++i)
	{
		m_CanvasMonster->RemoveChildAt(0);	
	}

	m_MonsterCount = 0;
}

void UMinimapWidget::AddMonster(const FVector& Loc)
{
	// 몬스터 좌표를 생성.
	UImage* Image = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass());

	Image->SetBrushTintColor(FLinearColor(1.f, 0.f, 0.f, 0.8f));

	UCanvasPanelSlot* CanvasSlot = m_CanvasMonster->AddChildToCanvas(Image);

	CanvasSlot->SetPosition(FVector2D(Loc.Y, 300.f - Loc.X));
	CanvasSlot->SetAlignment(FVector2D(1.f, 1.f));
	CanvasSlot->SetSize(FVector2D(5.f, 5.f));

	++m_MonsterCount;
}

void UMinimapWidget::AddUser(const FVector& Loc)
{
	UImage* Image = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass());

	Image->SetBrushTintColor(FLinearColor(0.27f, 0.55f, 0.63f, 0.8f));

	UCanvasPanelSlot* CanvasSlot = m_CanvasMonster->AddChildToCanvas(Image);

	CanvasSlot->SetPosition(FVector2D(Loc.Y, 300.f - Loc.X));
	CanvasSlot->SetAlignment(FVector2D(1.f, 1.f));
	CanvasSlot->SetSize(FVector2D(8.f, 8.f));

	m_MonsterCount++;
}
