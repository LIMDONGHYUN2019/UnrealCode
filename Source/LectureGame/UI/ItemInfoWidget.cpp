// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemInfoWidget.h"

void UItemInfoWidget::NativeConstruct()
{
	Super::NativeConstruct();

	m_Name = Cast<UTextBlock>(GetWidgetFromName(TEXT("Name")));
	m_Grade = Cast<UTextBlock>(GetWidgetFromName(TEXT("Grade")));
	m_Type = Cast<UTextBlock>(GetWidgetFromName(TEXT("Type")));
	m_Buy = Cast<UTextBlock>(GetWidgetFromName(TEXT("Buy")));
	m_Sell = Cast<UTextBlock>(GetWidgetFromName(TEXT("Sell")));

	m_Index = Cast<UTextBlock>(GetWidgetFromName(TEXT("Index")));

	m_Option = Cast<UTextBlock>(GetWidgetFromName(TEXT("Option")));
}

void UItemInfoWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}