// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestListWidget.h"
#include "QuestListData.h"

void UQuestListWidget::NativeConstruct()
{
	Super::NativeConstruct();

	m_Title = Cast<UTextBlock>(GetWidgetFromName(TEXT("Title")));
}

void UQuestListWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UQuestListWidget::SetListData(UQuestListData* Data)
{
	const _tagQuestInfo& Info = Data->GetQuestInfo();

	FString Name = UTF8_TO_TCHAR(Info.strName);

	m_Title->SetText(FText::FromString(Name));
}
