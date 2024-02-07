#pragma once

#include "../../GameInfo.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Blueprint/UserWidget.h"
#include "PlayerStatusWidget.generated.h"


UCLASS()
class LECTUREGAME_API UPlayerStatusWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UTextBlock* pName;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UTextBlock* pLevel;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UTextBlock* pJob;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UTextBlock* pAttack;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UTextBlock* pArmor;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UTextBlock* pHp;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UTextBlock* pMp;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UTextBlock* pAttackSpeed;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UTextBlock* pMoveSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UButton* pCloseButton;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UButton* pTest;

public:
	virtual void NativeConstruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime); // update

public:
	UFUNCTION()
	void ClosePlayerStatus();

	void SetStatus(_tagCharacterInfo* pCharacterInfo);
	void SetName(FString strname);
	void SetLevel(int32 ilevel);
	void SetJob(FString strjob);
	void SetAttack(int32 iattack);
	void SetArmor(int32 iarmor);
	void SetHp(int32 ihp);
	void SetMp(int32 imp);
	void SetAttackSpeed(int32 iattackspeed);
	void SetMoveSpeed(int32 imovespeed);
};
