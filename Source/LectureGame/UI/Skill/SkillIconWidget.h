// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../GameInfo.h"
#include "SkillInfoWidget.h"
#include "Components/Button.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"

#include "Components/CanvasPanelSlot.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Blueprint/UserWidget.h"
#include "SkillIconWidget.generated.h"

/**
 * 
 */
UCLASS()
class LECTUREGAME_API USkillIconWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UBorder* IconImage;

	SkillInfo* IconInfo;
	int32		SkillLevel;

	UTexture2D* Tempo;

public:
	virtual void NativeConstruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime); // update

public:
	void SetIconImage(UTexture2D* Texture)
	{
		IconImage->SetBrushFromTexture(Texture);

		Tempo = Texture;
	}

	void SetSkillInfo(SkillInfo* Info, int32 Level)
	{
		IconInfo = Info;
		SkillLevel = Level;
	}
	UTexture2D* GetIconImage() const
	{
		return Tempo;
	}

	SkillInfo* GetSkillInfo()const
	{
		return IconInfo;
	}

	int32 GetSkillLevel()	const
	{
		return SkillLevel;
	}

	UFUNCTION()
	FEventReply MouseButtonUp(FGeometry MyGeometry, const FPointerEvent& MouseEvent);
};
