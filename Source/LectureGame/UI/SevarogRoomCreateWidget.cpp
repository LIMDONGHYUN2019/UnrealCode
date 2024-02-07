// Fill out your copyright n otice in the Description page of Project Settings.


#include "SevarogRoomCreateWidget.h"

#include "../NetStream.h"
#include "../PacketSession.h"
#include "../GameServerManager.h"
#include "../Player/PlayerCharacter.h"


void USevarogRoomCreateWidget::NativeConstruct()
{
	Super::NativeConstruct();

	m_pConfirmButton = Cast<UButton>(GetWidgetFromName(TEXT("Confirm")));
	m_pCancelButton = Cast<UButton>(GetWidgetFromName(TEXT("Cancel")));

	m_pConfirmButton->OnClicked.AddDynamic(this, &USevarogRoomCreateWidget::Confirm);
	m_pCancelButton->OnClicked.AddDynamic(this, &USevarogRoomCreateWidget::Cancel);
}

void USevarogRoomCreateWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void USevarogRoomCreateWidget::Confirm()
{
	APlayerCharacter* pPlayer = GetWorld()->GetFirstPlayerController()->GetPawn<APlayerCharacter>();
	if (pPlayer->IsGroupAdmin())
	{
		uint8	strPacket[PACKET_LENGTH] = {};

		PacketSession* Session = GameServerManager::GetInst()->GetSession();
		Session->Write(GP_SEVAROGBOSSROOM, 0, strPacket);
	}
	else
	{
		PrintViewport(20.f, FColor::Blue, FString::Printf(TEXT("Not a Admin Player.......")));
	}
}

void USevarogRoomCreateWidget::Cancel()
{
	SetVisibility(ESlateVisibility::Collapsed);
}