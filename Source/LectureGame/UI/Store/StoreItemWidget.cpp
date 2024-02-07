// Fill out your copyright notice in the Description page of Project Settings.


#include "StoreItemWidget.h"
#include "ItemData.h"
#include "../../MainGameInstance.h"

void UStoreItemWidget::NativeConstruct()
{
	Super::NativeConstruct();

	IconImage = Cast<UImage>(GetWidgetFromName(TEXT("Icon")));
	NameText = Cast<UTextBlock>(GetWidgetFromName(TEXT("NameText")));
	GradeText = Cast<UTextBlock>(GetWidgetFromName(TEXT("GradeText")));
	PriceText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Price")));
}

void UStoreItemWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UStoreItemWidget::SetData(UItemData* pData)
{
	NameText->SetText(FText::FromString(pData->GetItemName()));

	FString Price = FString::Printf(TEXT("%d $$"), pData->GetPrice());
	PriceText->SetText(FText::FromString(Price));

	IconImage->SetBrushFromTexture(pData->GetIconTexture());

	FSlateColor color;
	switch (pData->GetItemGrade())
	{
	case EItemGrade::Normal:
	{ // 하얀색
		GradeText->SetText(FText::FromString(TEXT("일반 등급")));
		color = FSlateColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
		NameText->SetColorAndOpacity(color);
		GradeText->SetColorAndOpacity(color);
	}
	break;
	case EItemGrade::Rare:
	{ // 파란색
		GradeText->SetText(FText::FromString(TEXT("레어 등급")));
		color = FSlateColor(FLinearColor(0.086624f, 0.322002f, 1.0f, 1.f));
		NameText->SetColorAndOpacity(color);
		GradeText->SetColorAndOpacity(color);
	}
	break;
	case EItemGrade::Epic:
	{ // 노랑색
		GradeText->SetText(FText::FromString(TEXT("에픽 등급")));
		color = FSlateColor(FLinearColor(0.286458f, 0.225038f, 0.117523f, 1.f));
		NameText->SetColorAndOpacity(color);
		GradeText->SetColorAndOpacity(color);
	}
	break;
	case EItemGrade::Unique:
	{// 빨강색
		GradeText->SetText(FText::FromString(TEXT("유니크 등급")));
		color = FSlateColor(FLinearColor(1.f, 0.056774f, 0.086621f, 1.f));
		NameText->SetColorAndOpacity(color);
		GradeText->SetColorAndOpacity(color);
	}
	break;
	case EItemGrade::Legendary:
	{ // 주황색
		GradeText->SetText(FText::FromString(TEXT("레전더리 등급")));
		color = FSlateColor(FLinearColor(1.f, 0.464488f, 0.031026f, 1.f));
		NameText->SetColorAndOpacity(color);
		GradeText->SetColorAndOpacity(color);
	}
	break;
	default:
		break;
	}
}