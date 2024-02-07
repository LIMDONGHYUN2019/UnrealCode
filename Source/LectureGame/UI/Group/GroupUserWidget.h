// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../GameInfo.h"
#include "Components/TextBlock.h"
#include "GroupUserData.h"
#include "Blueprint/UserWidget.h"
#include "GroupUserWidget.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class LECTUREGAME_API UGroupUserWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UTextBlock* pName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UTextBlock* pLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UTextBlock* pJob;
	      
public:
	virtual void NativeConstruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime); // update

public:
	UFUNCTION(BlueprintCallable)
		void SetUser(UGroupUserData* pData);
	// 왜 name, level, job를 인식을 못하는지?
	// 그렇다면 addItem하면 

};
