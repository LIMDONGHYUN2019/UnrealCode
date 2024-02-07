// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../GameInfo.h"
#include "Components/Button.h"
#include "Blueprint/UserWidget.h"
#include "PlayerInfoWidget.generated.h"

/**
 * 
 */
UCLASS()
class LECTUREGAME_API UPlayerInfoWidget : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UButton* pTradeButton;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UButton* pPlayerStatusButton;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UButton* pGroupButton;

	class AUserCharacter* pOtherPlayer;

public:
	virtual void NativeConstruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime); // update
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);

public:
	void SetLocationFromMouse();
	void SetLocationFromMouse(const FVector2D& MousePos);
	void SetOtherPlayer(class AUserCharacter* OtherPlayer);
	AUserCharacter* GetOtherPlayer();

	UFUNCTION()
		void OpenTrade();
	UFUNCTION()
		void InvGroup();
	UFUNCTION()
		void OpenStatus();
};
