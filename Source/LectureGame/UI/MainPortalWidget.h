// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Components/Button.h"
#include "Blueprint/UserWidget.h"
#include "MainPortalWidget.generated.h"

/**
 * 
 */
UCLASS()
class LECTUREGAME_API UMainPortalWidget : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UButton* m_pConfirmButton;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UButton* m_pCancelButton;

	ELevel		m_LevelType;

public:
	virtual void NativeConstruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

	void SetLevelType(ELevel type)
	{
		m_LevelType = type;
	}

public:
	UFUNCTION()
		void Confirm();
	UFUNCTION()
		void Cancel();
};
