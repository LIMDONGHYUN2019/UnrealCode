// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Blueprint/UserWidget.h"
#include "MinimapWidget.generated.h"

/**
 * 
 */
UCLASS()
class LECTUREGAME_API UMinimapWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UCanvasPanel*	m_CanvasMonster;
	int32			m_MonsterCount;

public:
	virtual void NativeConstruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime); // update
	
public:
	void ClearMonster();
	void AddMonster(const FVector& Loc);
	void AddUser(const FVector& Loc);
};
