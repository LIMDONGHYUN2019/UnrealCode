// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectStatusWidget.h"
#include "Components/TextBlock.h"


void USelectStatusWidget::NativeConstruct()
{
	Super::NativeConstruct();

	JobText = Cast<UTextBlock>(GetWidgetFromName(TEXT("JobText")));
	AttackText = Cast<UTextBlock>(GetWidgetFromName(TEXT("AttackText")));
	ArmorText = Cast<UTextBlock>(GetWidgetFromName(TEXT("ArmorText")));
	AttackSpeedText = Cast<UTextBlock>(GetWidgetFromName(TEXT("AttackSpeedText")));
	MoveSpeedText = Cast<UTextBlock>(GetWidgetFromName(TEXT("MoveSpeedText")));
	LevelText = Cast<UTextBlock>(GetWidgetFromName(TEXT("LevelText")));
	HPText = Cast<UTextBlock>(GetWidgetFromName(TEXT("HPText")));
	MPText = Cast<UTextBlock>(GetWidgetFromName(TEXT("MPText")));
}

void USelectStatusWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void USelectStatusWidget::SetJob(const FString& strJob)
{
	JobText->SetText(FText::FromString(strJob));
}

void USelectStatusWidget::SetAttack(int32 iAttack)
{
	FString	str = FString::Printf(TEXT("%.d"), iAttack);
	AttackText->SetText(FText::FromString(str));
}

void USelectStatusWidget::SetArmor(int32 iArmor)
{
	FString	str = FString::Printf(TEXT("%.d"), iArmor);
	ArmorText->SetText(FText::FromString(str));
}

void USelectStatusWidget::SetAttackSpeed(float fSpeed)
{
	FString	str = FString::Printf(TEXT("%.2f"), fSpeed);
	AttackSpeedText->SetText(FText::FromString(str));
}

void USelectStatusWidget::SetMoveSpeed(float fSpeed)
{
	FString	str = FString::Printf(TEXT("%.2f"), fSpeed);
	MoveSpeedText->SetText(FText::FromString(str));
}

void USelectStatusWidget::SetHP(float fHp)
{
	FString	str = FString::Printf(TEXT("%.2f"), fHp);
	HPText->SetText(FText::FromString(str));
}

void USelectStatusWidget::SetMP(float fMp)
{
	FString	str = FString::Printf(TEXT("%.2f"), fMp);
	MPText->SetText(FText::FromString(str));
}

void USelectStatusWidget::SetLevel(int32 fLevel)
{
	FString	str = FString::Printf(TEXT("%d"), fLevel);
	LevelText->SetText(FText::FromString(str));
}
