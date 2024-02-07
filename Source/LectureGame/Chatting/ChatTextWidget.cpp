// Fill out your copyright notice in the Description page of Project Settings.


#include "ChatTextWidget.h"
#include "Components/TextBlock.h"
#include "ChatData.h"

void UChatTextWidget::NativeConstruct()
{
	Super::NativeConstruct();

	m_Text = Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock")));
}

void UChatTextWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UChatTextWidget::SetData(class UChatData* pData)
{
	if (pData->GetNotice())
	{
		FString Message = pData->GetText();

		m_Text->SetText(FText::FromString(Message));

		FSlateColor color;
		color = FSlateColor(FLinearColor(1.f, 0.464488f, 0.031026f, 1.f));
		m_Text->SetColorAndOpacity(color);
	}
	else
	{
		SetChattingText(pData->GetText());
	}

}

void UChatTextWidget::SetChattingText(const FString& Text)
{
	m_Text->SetText(FText::FromString(Text));
}

