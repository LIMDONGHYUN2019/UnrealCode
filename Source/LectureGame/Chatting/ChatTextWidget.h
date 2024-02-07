// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Blueprint/UserWidget.h"
#include "ChatTextWidget.generated.h"

UCLASS()
class LECTUREGAME_API UChatTextWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UTextBlock* m_Text;
public:
	virtual void NativeConstruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime); // update

public:
	void SetChattingText(const FString& Text);
public:
	UFUNCTION(BlueprintCallable)
	void SetData(class UChatData* pData);
};
