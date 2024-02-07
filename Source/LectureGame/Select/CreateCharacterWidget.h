// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Blueprint/UserWidget.h"
#include "CreateCharacterWidget.generated.h"

UCLASS()
class LECTUREGAME_API UCreateCharacterWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UButton* CreateCharacterButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UButton* BackButton;

	// UI_SelectStatus
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class USelectStatusWidget*	StatusWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UEditableTextBox* InputName;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UUserWidget* CreateFailMsgBox;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UUserWidget* CreateSuccessMsgBox;

public:
	class USelectStatusWidget* GetStatusWidget()	const
	{
		return StatusWidget;
	}

public:
	virtual void NativeConstruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime); // update

public:
	// 선택창에서 버튼들.
	UFUNCTION()
	void CreateCharacter();

	UFUNCTION()
	void Back();

	void EnableInputName(bool bEnable);
	FString GetInputName();
	void ResetInputName();
	void PopUpCreateFailMsgBox();
	void PopUpCreateSuccessMsgBox();
};
