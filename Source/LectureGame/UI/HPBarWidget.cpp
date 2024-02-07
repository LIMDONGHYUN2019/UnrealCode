// Fill out your copyright notice in the Description page of Project Settings.


#include "HPBarWidget.h"
#include "Components/TextBlock.h"

void UHPBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HPBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("HPBar")));
	MPBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("MPBar")));
	pName = Cast<UTextBlock>(GetWidgetFromName(TEXT("NameText")));

	if (m_BeginCallback.IsBound())
		m_BeginCallback.Execute();

}

void UHPBarWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}
void UHPBarWidget::SetHPPercent(float Percent)
{
	HPBar->SetPercent(Percent);
}
void UHPBarWidget::SetMPPercent(float Percent)
{
	MPBar->SetPercent(Percent);
}
void UHPBarWidget::SetCharacterName(const FString& str)
{
	if (pName)
		pName->SetText(FText::FromString(str));
}
