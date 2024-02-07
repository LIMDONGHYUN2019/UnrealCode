// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Blueprint/UserWidget.h"
#include "ChattingWidget.generated.h"

/*
 
 */
UCLASS()
class LECTUREGAME_API UChattingWidget :
	public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UListView* ChatList;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UEditableTextBox* ChatInput;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UButton* SendButton;

public:
	virtual void NativeConstruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime); // update

public:
	void AddChatting(FString Text, bool Notice = false);

protected:
	UFUNCTION()
	void ChatCommit(const FText& Text, ETextCommit::Type CommitType);

	UFUNCTION()
	void SendButtonClick();

private:
	void SendChatText();
};

