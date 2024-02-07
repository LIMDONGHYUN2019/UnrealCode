// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPortalWidget.h"

#include "../NetStream.h"
#include "../PacketSession.h"
#include "../GameServerManager.h"
#include "../MainGameInstance.h"
#include "../Player/PlayerCharacter.h"

void UMainPortalWidget::NativeConstruct()
{
	Super::NativeConstruct();

	m_pConfirmButton = Cast<UButton>(GetWidgetFromName(TEXT("Confirm")));
	m_pCancelButton = Cast<UButton>(GetWidgetFromName(TEXT("Cancel")));

	m_pConfirmButton->OnClicked.AddDynamic(this, &UMainPortalWidget::Confirm);
	m_pCancelButton->OnClicked.AddDynamic(this, &UMainPortalWidget::Cancel);
}

void UMainPortalWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UMainPortalWidget::Confirm()
{
	uint8	strPacket[PACKET_LENGTH] = {};

	PacketSession* pSession = GameServerManager::GetInst()->GetSession();
	// 어떤 레벨로 전환이 일어나야 하는지를 레벨타입을 패킷에 추가하여 보내준다.

	NetStream stream;
	stream.SetBuffer(strPacket);

	int32 iLevelType = (int32)m_LevelType;
	stream.Write(&iLevelType, sizeof(int32));

	pSession->Write(GP_MAINPORTAL, stream.GetLength(), strPacket);

	SetVisibility(ESlateVisibility::Collapsed);
}

void UMainPortalWidget::Cancel()
{
	SetVisibility(ESlateVisibility::Collapsed);
}