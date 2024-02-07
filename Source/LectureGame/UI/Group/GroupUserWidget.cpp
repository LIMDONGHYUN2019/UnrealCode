// Fill out your copyright notice in the Description page of Project Settings.


#include "GroupUserWidget.h"

void UGroupUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	pName = Cast<UTextBlock>(GetWidgetFromName(TEXT("Name")));
	pJob = Cast<UTextBlock>(GetWidgetFromName(TEXT("Job")));
	pLevel = Cast<UTextBlock>(GetWidgetFromName(TEXT("Level")));
}

void UGroupUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UGroupUserWidget::SetUser(UGroupUserData* pData)
{
	FString Name = pData->GetName();
	FString Job = pData->GetJob();
	FString Level = pData->GetLevel();

	pName->SetText(FText::FromString(Name));
	pJob->SetText(FText::FromString(Job));
	pLevel->SetText(FText::FromString(Level));

	/*this->SetColorAndOpacity;*/
}