// Fill out your copyright notice in the Description page of Project Settings.


#include "InformationWidget.h"


void UInformationWidget::NativeConstruct()
{
	Super::NativeConstruct();

	m_pCloseButton = Cast<UButton>(GetWidgetFromName(TEXT("CloseButton")));

	m_pCloseButton->OnClicked.AddDynamic(this, &UInformationWidget::CloseInformation);
}

void UInformationWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UInformationWidget::CloseInformation()
{
	SetVisibility(ESlateVisibility::Collapsed);
}