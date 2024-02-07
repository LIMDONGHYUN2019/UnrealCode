#pragma once

#include "../../GameInfo.h"
#include "Blueprint/UserWidget.h"
#include "TradeRequestWidget.generated.h"

/**
 * 
 */
UCLASS()
class LECTUREGAME_API UTradeRequestWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UButton* pConfirmButton;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UButton* pCancelButton;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UTextBlock* RequestName;

	FString RequestPlayerName;
	int32 RequestPlayerNumber;

public:
	virtual void NativeConstruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

public:
	UFUNCTION()
	void OpenTrade();

	UFUNCTION()
	void Close();

	void Open();
	void SetRequestDetails(FString playername, int32& characternumber);
};
