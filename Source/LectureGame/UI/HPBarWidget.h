// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Components/ProgressBar.h"
#include "Blueprint/UserWidget.h"
#include "HPBarWidget.generated.h"

DECLARE_DELEGATE(FBeginCallback);

UCLASS()
class LECTUREGAME_API UHPBarWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UProgressBar* HPBar;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UProgressBar* MPBar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UTextBlock* pName;

	FBeginCallback	m_BeginCallback;
	FBeginCallback	m_BeginPlayer;

public:
	virtual void NativeConstruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime); // update

public:
	void SetHPPercent(float Percent);
	void SetMPPercent(float Percent);
	void SetCharacterName(const FString& str);

public:
	template <typename T>
	void SetBeginCallback(T* pObj, void (T::* pFunc)())
	{
		m_BeginCallback.BindUObject(pObj, pFunc);
	}
};
