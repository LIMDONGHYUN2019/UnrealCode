// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../GameInfo.h"
#include "Components/ListView.h"
#include "Components/Button.h"
#include "Blueprint/UserWidget.h"
#include "GroupWidget.generated.h"

/**
 * 
 */
UCLASS()
class LECTUREGAME_API UGroupWidget : public UUserWidget
{
	GENERATED_BODY()
	
private:
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	//UListView* GroupList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UButton* pLeave;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UButton* pCreate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UListView* pGroupList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UButton* pCloseButton;

	class APlayerCharacter* pAdminPlayer;

public:
	virtual void NativeConstruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime); // update

public:
	UFUNCTION()
		void Leave();
	UFUNCTION()
		void Create();
	UFUNCTION()
		void CloseGroup();
public:
	void Open()
	{
		pCreate->SetVisibility(ESlateVisibility::Collapsed);
		pLeave->SetVisibility(ESlateVisibility::Visible);
	}
	void AddPlayer(FString Name, FString Job, int32& Level);
	void LeavePlayer();
};
