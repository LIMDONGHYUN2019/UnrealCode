// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStatusWidget.h"
#include "../../MainGameInstance.h"

void UPlayerStatusWidget::NativeConstruct()
{
	Super::NativeConstruct();

	pName = Cast<UTextBlock>(GetWidgetFromName(TEXT("Name")));
	pLevel = Cast<UTextBlock>(GetWidgetFromName(TEXT("Level")));
	pJob = Cast<UTextBlock>(GetWidgetFromName(TEXT("Job")));
	pAttack = Cast<UTextBlock>(GetWidgetFromName(TEXT("Attack")));
	pArmor = Cast<UTextBlock>(GetWidgetFromName(TEXT("Armor")));
	pHp = Cast<UTextBlock>(GetWidgetFromName(TEXT("Hp")));
	pMp = Cast<UTextBlock>(GetWidgetFromName(TEXT("Mp")));
	pAttackSpeed = Cast<UTextBlock>(GetWidgetFromName(TEXT("AttackSpeed")));
	pMoveSpeed = Cast<UTextBlock>(GetWidgetFromName(TEXT("MoveSpeed")));
	
	pCloseButton = Cast<UButton>(GetWidgetFromName(TEXT("CloseButton")));
	pCloseButton->OnClicked.AddDynamic(this, &UPlayerStatusWidget::ClosePlayerStatus);

}

void UPlayerStatusWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UPlayerStatusWidget::ClosePlayerStatus()
{
	SetVisibility(ESlateVisibility::Collapsed);
}

void UPlayerStatusWidget::SetStatus(_tagCharacterInfo* pCharacterInfo)
{
	pName->SetText(FText::FromString(ANSI_TO_TCHAR(pCharacterInfo->strName)));

	pJob->SetText(FText::FromString(ANSI_TO_TCHAR(pCharacterInfo->strJob)));

	FString level = FString::Printf(TEXT("%d"), pCharacterInfo->iLevel);
	pLevel->SetText(FText::FromString(level));

	FString attack = FString::Printf(TEXT("%d"), pCharacterInfo->iAttack);
	pAttack->SetText(FText::FromString(attack));

	FString armor = FString::Printf(TEXT("%d"), pCharacterInfo->iArmor);
	pArmor->SetText(FText::FromString(armor));

	FString hpmax = FString::Printf(TEXT("%d"), pCharacterInfo->iHPMax);
	pHp->SetText(FText::FromString(hpmax));

	FString mpmax = FString::Printf(TEXT("%d"), pCharacterInfo->iMPMax);
	pMp->SetText(FText::FromString(mpmax));

	FString attackspeed = FString::Printf(TEXT("%.2f"), pCharacterInfo->fAttackSpeed);
	pAttackSpeed->SetText(FText::FromString(attackspeed));

	FString movespeed = FString::Printf(TEXT("%.2f"), pCharacterInfo->fMoveSpeed);
	pMoveSpeed->SetText(FText::FromString(movespeed));
}

void UPlayerStatusWidget::SetName(FString strname)
{
	pName->SetText(FText::FromString(strname));
}

void UPlayerStatusWidget::SetLevel(int32 ilevel)
{
	FString level = FString::Printf(TEXT("%d"), ilevel);
	pLevel->SetText(FText::FromString(level));
}

void UPlayerStatusWidget::SetJob(FString strjob)
{
	pJob->SetText(FText::FromString(strjob));
}

void UPlayerStatusWidget::SetAttack(int32 iattack)
{
	FString attack = FString::Printf(TEXT("%d"), iattack);
	pAttack->SetText(FText::FromString(attack));
}

void UPlayerStatusWidget::SetArmor(int32 iarmor)
{
	FString armor = FString::Printf(TEXT("%d"), iarmor);
	pArmor->SetText(FText::FromString(armor));
}

void UPlayerStatusWidget::SetHp(int32 ihp)
{
	FString hp = FString::Printf(TEXT("%d"), ihp);
	pHp->SetText(FText::FromString(hp));
}

void UPlayerStatusWidget::SetMp(int32 imp)
{
	FString mp = FString::Printf(TEXT("%d"), imp);
	pMp->SetText(FText::FromString(mp));
}

void UPlayerStatusWidget::SetAttackSpeed(int32 iattackspeed)
{
	FString attackspeed = FString::Printf(TEXT("%d"), iattackspeed);
	pAttackSpeed->SetText(FText::FromString(attackspeed));
}

void UPlayerStatusWidget::SetMoveSpeed(int32 imovespeed)
{
	FString movespeed = FString::Printf(TEXT("%d"), imovespeed);
	pMoveSpeed->SetText(FText::FromString(movespeed));
}
