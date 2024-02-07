// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestWidget.h"
#include "QuestListData.h"

void UQuestWidget::NativeConstruct()
{
	Super::NativeConstruct();

	m_QuestList = Cast<UListView>(GetWidgetFromName(TEXT("QuestList")));
	m_CloseButton = Cast<UButton>(GetWidgetFromName(TEXT("QuestClose")));

	m_QuestList->OnItemClicked().AddUObject(this, &UQuestWidget::QuestClick);

	m_CloseButton->OnClicked.AddDynamic(this, &UQuestWidget::CloseButton);
}

void UQuestWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}
								
void UQuestWidget::AddQuest(const _tagQuestInfo& Info)
{
	UQuestListData* ListData = NewObject<UQuestListData>(this, UQuestListData::StaticClass());

	ListData->SetQuestInfo(Info);

	m_QuestList->AddItem(ListData);
}

void UQuestWidget::QuestClick(UObject* pObj)
{
	UQuestListData* ListData = (UQuestListData*)pObj;

	PrintViewport(1.f, FColor::Red, (TEXT("Quest : %s"), UTF8_TO_TCHAR(ListData->GetQuestInfo().strName)));
}

void UQuestWidget::CloseButton()
{
	if(GetVisibility() == ESlateVisibility::Visible)
		SetVisibility(ESlateVisibility::Collapsed);
}
