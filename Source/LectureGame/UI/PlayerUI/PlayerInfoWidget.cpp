#include "PlayerInfoWidget.h"
#include "TradeWidget.h"
#include "Components/CanvasPanelSlot.h"
#include "../Group/GroupWidget.h"
#include "../../GameMode/LectureGameGameModeBase.h"
#include "../../NetStream.h"
#include "../../NetSession.h"
#include "../../PacketSession.h"
#include "../../OtherUser/UserCharacter.h"
#include "../../GameServerManager.h"
				  
void UPlayerInfoWidget::NativeConstruct()
{
	Super::NativeConstruct();

	pTradeButton = Cast<UButton>(GetWidgetFromName(TEXT("Trade")));
	pGroupButton = Cast<UButton>(GetWidgetFromName(TEXT("InvGroup")));
	pPlayerStatusButton = Cast<UButton>(GetWidgetFromName(TEXT("Status")));

	pTradeButton->OnClicked.AddDynamic(this, &UPlayerInfoWidget::OpenTrade);
	pGroupButton->OnClicked.AddDynamic(this, &UPlayerInfoWidget::InvGroup);
	pPlayerStatusButton->OnClicked.AddDynamic(this, &UPlayerInfoWidget::OpenTrade);
}

void UPlayerInfoWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

FReply UPlayerInfoWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply	result = Super::NativeOnMouseMove(InGeometry, InMouseEvent);

	return result;
}

void UPlayerInfoWidget::SetLocationFromMouse()
{
	float	MouseX, MouseY;
	if (GetWorld()->GetFirstPlayerController()->GetMousePosition(MouseX, MouseY))
	{
		UCanvasPanelSlot* PanelSlot = Cast<UCanvasPanelSlot>(Slot);

		FVector2D	ScreenPos;
		FVector2D	ViewportPos;

		ScreenPos.X = MouseX;
		ScreenPos.Y = MouseY;
		USlateBlueprintLibrary::ScreenToViewport(this, ScreenPos, ViewportPos);
	}
}

void UPlayerInfoWidget::SetLocationFromMouse(const FVector2D& MousePos)
{
	UCanvasPanelSlot* PanelSlot = Cast<UCanvasPanelSlot>(Slot);

	PanelSlot->SetPosition(MousePos);
}

void UPlayerInfoWidget::SetOtherPlayer(AUserCharacter* Player)
{
	pOtherPlayer = Player;
}

AUserCharacter* UPlayerInfoWidget::GetOtherPlayer()
{
	return pOtherPlayer;
}

void UPlayerInfoWidget::OpenTrade()
{
	ALectureGameGameModeBase* pGame = Cast<ALectureGameGameModeBase>(GetWorld()->GetAuthGameMode());
	if (pGame)
	{
		UTradeWidget* pTrade = (pGame->GetGameWidget())->GetTradeWidget();

		APlayerCharacter* pPlayer = GetWorld()->GetFirstPlayerController()->GetPawn<APlayerCharacter>();

		if (pTrade)
		{
			NetStream stream;
			uint8 strPacket[PACKET_LENGTH] = {};
			stream.SetBuffer(strPacket);
			int32 OtherPlayerCharacterNumber = pOtherPlayer->GetCharacterNumber();
			stream.Write(&OtherPlayerCharacterNumber, sizeof(int32));

			PacketSession* pSession = GameServerManager::GetInst()->GetSession();
			pSession->Write(GP_TRADEREQUEST, stream.GetLength(), strPacket);

			SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UPlayerInfoWidget::OpenStatus()
{
	ALectureGameGameModeBase* pGame = Cast<ALectureGameGameModeBase>(GetWorld()->GetAuthGameMode());
	if (pGame)
	{
		/*UTradeWidget* pTrade = pGame->GetGameWidget()->GetTradeWidget();

		APlayerCharacter* pPlayer = GetWorld()->GetFirstPlayerController()->GetPawn<APlayerCharacter>();

		if (pTrade)
		{
			NetStream stream;
			uint8 strPacket[PACKET_LENGTH] = {};
			stream.SetBuffer(strPacket);
			int32 OtherPlayerCharacterNumber = pOtherPlayer->GetCharacterNumber();
			stream.Write(&OtherPlayerCharacterNumber, sizeof(int32));

			PacketSession* pSession = GameServerManager::GetInst()->GetSession();
			pSession->Write(GP_TRADEREQUEST, stream.GetLength(), strPacket);

			SetVisibility(ESlateVisibility::Collapsed);
		}*/
	}
}

void UPlayerInfoWidget::InvGroup()
{
	ALectureGameGameModeBase* pGame = Cast<ALectureGameGameModeBase>(GetWorld()->GetAuthGameMode());
	APlayerCharacter* pPlayer = GetWorld()->GetFirstPlayerController()->GetPawn<APlayerCharacter>();
	if (pGame)
	{
		if (pPlayer->GetIsGroup())
		{
			NetStream stream;
			uint8 strPacket[PACKET_LENGTH] = {};
			stream.SetBuffer(strPacket);
			int32 OtherPlayerCharacterNumber = pOtherPlayer->GetCharacterNumber();
			stream.Write(&OtherPlayerCharacterNumber, sizeof(int32));

			PacketSession* pSession = GameServerManager::GetInst()->GetSession();
			pSession->Write(GP_GROUPREQUEST, stream.GetLength(), strPacket);
		}
		else
		{
			PrintViewport(5.f, FColor::Green, FString::Printf(TEXT("Not a party Admin......")));
		}

		SetVisibility(ESlateVisibility::Collapsed);
	}
}
