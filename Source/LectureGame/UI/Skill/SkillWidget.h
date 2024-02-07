// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "../../GameInfo.h"
#include "SkillInfoWidget.h"
#include "Components/ListView.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"

#include "Components/CanvasPanelSlot.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"

#include "SkillWidget.generated.h"

UCLASS()
class LECTUREGAME_API USkillWidget : public UUserWidget
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UButton* CloseButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UButton* TitlebarButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<UButton*>	SkillButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<UTextBlock*>	SkillLevelText; // 현재 스킬들 레벨

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UTextBlock* PointText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USkillInfoWidget* SkillInfoWidget;

	int32		m_SkillPoint;// 안찍은 포인트.

	bool		m_bMouseHovered;
	bool		m_bMousePressed;
	bool		m_bMousePressStart;
	bool		m_bMouseDrag;

	// 스킬을 드래그하는건지 타이틀바를 드래그하는건지 구분을 해야한다.
	bool		m_bSkillIconDrag;
	bool		m_bTitlebarDrag;

	FVector2D	PressStartPos;
	FVector2D	m_Distance;

	TArray<SkillInfo*>	m_SkillInfoArray;
	TArray<UTexture2D*>	m_SkillIconImageArray;

	UCanvasPanel* m_CanvasSkill;
	int32		m_DragSkillIndex;

public:
	SkillInfo* GetSelectSkillInfo()
	{
		if (m_DragSkillIndex == -1)
			return nullptr;

		return m_SkillInfoArray[m_DragSkillIndex];
	}

	UTexture2D* GetSelectSkillIconImage()
	{
		if (m_DragSkillIndex == -1)
			return nullptr;

		return m_SkillIconImageArray[m_DragSkillIndex];
	}

	TArray<SkillInfo*>* GetSkillInfo()
	{
		return &m_SkillInfoArray;
	}

	bool	GetSkillIconDrag()	const
	{
		return m_bSkillIconDrag;
	}
	bool	GetTitlebarDrag()	const
	{
		return m_bTitlebarDrag;
	}

	FVector2D GetDistance()	const
	{
		return m_Distance;
	}

	int32 GetSkillLevel()
	{
		return 0;
	}

public:
	void AddSkillInfo(SkillInfo* Info)
	{
		m_SkillInfoArray.Add(Info);
	}

public:
	bool IsDrag()	const
	{
		return m_bMouseDrag;
	}

public:
	void SetSkillPoint(int32 Point);
	void SkillPointUp(int32 Point);
	void SetSkillInfoWidget(int32 Index);

public:
	void ChangeIcon(int32 Index, UTexture2D* Image);
	void ChangeSkillLevel(int32 Index, int32 Level);

public:
	virtual void NativeConstruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime); // update

	UFUNCTION()
		void CloseSkill();
	UFUNCTION()
		void TilePressed();
	UFUNCTION()
		void TileReleased();

	UFUNCTION()
		void SkillIcon1_1();

	UFUNCTION()
		void SkillIcon1_2();

	UFUNCTION()
		void SkillIcon1_3();

	UFUNCTION()
		void SkillIcon2_1();

	UFUNCTION()
		void SkillIcon2_2();

	UFUNCTION()
		void SkillIcon2_3();

	UFUNCTION()
		void SkillIcon2_4();

	UFUNCTION()
		void SkillIcon3_1();

	UFUNCTION()
		void SkillIcon3_2();

	UFUNCTION()
		void SkillIcon3_3();

	UFUNCTION()
		void SkillIcon1_1_Hovered();

	UFUNCTION()
		void SkillIcon1_2_Hovered();

	UFUNCTION()
		void SkillIcon1_3_Hovered();

	UFUNCTION()
		void SkillIcon2_1_Hovered();

	UFUNCTION()
		void SkillIcon2_2_Hovered();

	UFUNCTION()
		void SkillIcon2_3_Hovered();

	UFUNCTION()
		void SkillIcon2_4_Hovered();

	UFUNCTION()
		void SkillIcon3_1_Hovered();

	UFUNCTION()
		void SkillIcon3_2_Hovered();

	UFUNCTION()
		void SkillIcon3_3_Hovered();

	UFUNCTION()
		void SkillIcon1_1_UnHovered();

	UFUNCTION()
		void SkillIcon1_2_UnHovered();

	UFUNCTION()
		void SkillIcon1_3_UnHovered();

	UFUNCTION()
		void SkillIcon2_1_UnHovered();

	UFUNCTION()
		void SkillIcon2_2_UnHovered();

	UFUNCTION()
		void SkillIcon2_3_UnHovered();

	UFUNCTION()
		void SkillIcon2_4_UnHovered();

	UFUNCTION()
		void SkillIcon3_1_UnHovered();

	UFUNCTION()
		void SkillIcon3_2_UnHovered();

	UFUNCTION()
		void SkillIcon3_3_UnHovered();

	UFUNCTION()
		void SkillIcon1_1_Pressed();

	UFUNCTION()
		void SkillIcon1_2_Pressed();

	UFUNCTION()
		void SkillIcon1_3_Pressed();

	UFUNCTION()
		void SkillIcon2_1_Pressed();

	UFUNCTION()
		void SkillIcon2_2_Pressed();

	UFUNCTION()
		void SkillIcon2_3_Pressed();

	UFUNCTION()
		void SkillIcon2_4_Pressed();

	UFUNCTION()
		void SkillIcon3_1_Pressed();

	UFUNCTION()
		void SkillIcon3_2_Pressed();

	UFUNCTION()
		void SkillIcon3_3_Pressed();

	UFUNCTION()
		void SkillIcon1_1_Released();

	UFUNCTION()
		void SkillIcon1_2_Released();

	UFUNCTION()
		void SkillIcon1_3_Released();

	UFUNCTION()
		void SkillIcon2_1_Released();

	UFUNCTION()
		void SkillIcon2_2_Released();

	UFUNCTION()
		void SkillIcon2_3_Released();

	UFUNCTION()
		void SkillIcon2_4_Released();

	UFUNCTION()
		void SkillIcon3_1_Released();

	UFUNCTION()
		void SkillIcon3_2_Released();

	UFUNCTION()
		void SkillIcon3_3_Released();
};
