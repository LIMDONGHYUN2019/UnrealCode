// Fill out your copyright notice in the Description page of Project Settings.


#include "TradeRequestWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "TradeWidget.h"
#include "../../GameMode/LectureGameGameModeBase.h"
#include "../../Player/PlayerCharacter.h"

#include "../../NetStream.h"
#include "../../PacketSession.h"
#include "../../GameServerManager.h"
				  
void UTradeRequestWidget::NativeConstruct()
{
	Super::NativeConstruct();

	pConfirmButton = Cast<UButton>(GetWidgetFromName(TEXT("Confirm")));
	pCancelButton = Cast<UButton>(GetWidgetFromName(TEXT("Cancel")));
	RequestName = Cast<UTextBlock>(GetWidgetFromName(TEXT("RequestName")));
	
	pConfirmButton->OnClicked.AddDynamic(this, &UTradeRequestWidget::OpenTrade);
	pCancelButton->OnClicked.AddDynamic(this, &UTradeRequestWidget::Close);
}

void UTradeRequestWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UTradeRequestWidget::OpenTrade()
{
	ALectureGameGameModeBase* pGame = Cast<ALectureGameGameModeBase>(GetWorld()->GetAuthGameMode());
	if (pGame)
	{
		UTradeWidget* pTrade = pGame->GetGameWidget()->GetTradeWidget();
		APlayerCharacter* pPlayer = GetWorld()->GetFirstPlayerController()->GetPawn<APlayerCharacter>();
		if (pTrade)
		{
			pGame->GetGameWidget()->OpenTradeWidget();
			pGame->GetGameWidget()->OpenInventoryWidget();
			

			FString MyPlayerName = pPlayer->GetCharacterInfo().strName;
			pTrade->SetPlayerName(MyPlayerName);
			pTrade->SetOtherPlayerName(RequestPlayerName);
			pTrade->SetOtherPlayerCharacterNumber(RequestPlayerNumber);

			//===================================================================

			NetStream stream;
			uint8 strPacket[PACKET_LENGTH] = {};
			stream.SetBuffer(strPacket);

			stream.Write(&RequestPlayerNumber, sizeof(int32));

			PacketSession* pSession = GameServerManager::GetInst()->GetSession();
			pSession->Write(GP_TRADEREQUESTACCEPT, stream.GetLength(), strPacket);


			SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UTradeRequestWidget::Close()
{
	SetVisibility(ESlateVisibility::Collapsed);
}

void UTradeRequestWidget::Open()
{
	SetVisibility(ESlateVisibility::Visible);
}

void UTradeRequestWidget::SetRequestDetails(FString Playername, int32& Characternumber)
{
	RequestPlayerName = Playername;
	RequestPlayerNumber = Characternumber;

	RequestName->SetText(FText::FromString( RequestPlayerName + TEXT(" 에서 거래요청이 왔습니다.")));
}


