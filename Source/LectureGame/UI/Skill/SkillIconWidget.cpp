// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillIconWidget.h"


void USkillIconWidget::NativeConstruct()
{
	Super::NativeConstruct();

	IconImage = Cast<UBorder>(GetWidgetFromName(TEXT("Icon")));
	IconImage->OnMouseButtonUpEvent.BindUFunction(this, TEXT("MouseButtonUp"));
}

void USkillIconWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

FEventReply USkillIconWidget::MouseButtonUp(FGeometry MyGeometry, const FPointerEvent& MouseEvent)
{
	PrintViewport(20.f, FColor::Red, TEXT("Icon Mouse Up"));

	return FEventReply();
}
