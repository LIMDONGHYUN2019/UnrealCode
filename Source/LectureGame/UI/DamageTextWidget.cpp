// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageTextWidget.h"

void UDamageTextWidget::NativeConstruct()
{
	Super::NativeConstruct();

	pDamageText = Cast<UTextBlock>(GetWidgetFromName(TEXT("DamageText")));

	DmgStart = false;
}

void UDamageTextWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	DmgStart = true;
}

 