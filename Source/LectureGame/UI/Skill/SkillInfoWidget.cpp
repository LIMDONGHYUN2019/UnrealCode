// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillInfoWidget.h"

void USkillInfoWidget::NativeConstruct()
{
	Super::NativeConstruct();

	m_Name = Cast<UTextBlock>(GetWidgetFromName(TEXT("Name")));
	m_Type = Cast<UTextBlock>(GetWidgetFromName(TEXT("Type")));
	m_Property = Cast<UTextBlock>(GetWidgetFromName(TEXT("Property")));
	m_SkillLevel = Cast <UTextBlock>(GetWidgetFromName(TEXT("Level")));
	m_SkillLevelMax = Cast <UTextBlock>(GetWidgetFromName(TEXT("MaxLevel")));
	m_RequiredLevel = Cast<UTextBlock>(GetWidgetFromName(TEXT("RequiredLevel")));
	m_Cooldown = Cast<UTextBlock>(GetWidgetFromName(TEXT("Cooldown")));
	m_Desc = Cast<UTextBlock>(GetWidgetFromName(TEXT("Desc")));

	//-------------------------------------------------------------

	m_Option = Cast<UTextBlock>(GetWidgetFromName(TEXT("Option")));
}

void USkillInfoWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}