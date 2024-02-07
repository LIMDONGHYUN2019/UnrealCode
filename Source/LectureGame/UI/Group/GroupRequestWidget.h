// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../GameInfo.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Blueprint/UserWidget.h"
#include "GroupRequestWidget.generated.h"

/**
 * 
 */
UCLASS()
class LECTUREGAME_API UGroupRequestWidget : public UUserWidget
{
	GENERATED_BODY()
	

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UButton* pAccept;
	UPROPERTY(EditAnywhere, blueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UButton* pCancel;
	UPROPERTY(EditAnywhere, blueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UTextBlock* pName;


	int32 RequestCharacterNumber;

public:
	virtual void NativeConstruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);


public:
	void SetRequestCharacter(int32 CharacterNumber, FString name)
	{
		RequestCharacterNumber = CharacterNumber;
		pName->SetText(FText::FromString(name));
	}

	UFUNCTION()
	void Accept();
	UFUNCTION()
	void Cancel();
};
