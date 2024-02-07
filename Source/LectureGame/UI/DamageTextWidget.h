// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"
#include "DamageTextWidget.generated.h"

/**
 * 
 */
UCLASS()
class LECTUREGAME_API UDamageTextWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UTextBlock* pDamageText;

	bool	DmgStart;

public:
	bool IsStart()
	{
		return DmgStart;
	}


protected:
	virtual void NativeConstruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime); // update

public:
	void SetDamageText(int32 Damage)
	{
		pDamageText->SetText(FText::FromString(FString::Printf(TEXT("%d"), Damage)));
	}

	
};
