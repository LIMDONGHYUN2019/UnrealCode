// Fill out your copyright notice in the Description page of Project Settings.


#include "GroupRequestWidget.h"
#include "Components/Button.h"
#include "../../NetStream.h"
#include "../../PacketSession.h"
#include "../../GameServerManager.h"

void UGroupRequestWidget::NativeConstruct()
{
	Super::NativeConstruct();

	pAccept = Cast<UButton>(GetWidgetFromName(TEXT("Accept")));
	pCancel = Cast<UButton>(GetWidgetFromName(TEXT("Cancel")));
	pName = Cast<UTextBlock>(GetWidgetFromName(TEXT("Name")));
	
	pAccept->OnClicked.AddDynamic(this, &UGroupRequestWidget::Accept);
	pCancel->OnClicked.AddDynamic(this, &UGroupRequestWidget::Cancel);
}

void UGroupRequestWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UGroupRequestWidget::Accept()
{
	NetStream stream;
	uint8 strPacket[PACKET_LENGTH] = {};
	stream.SetBuffer(strPacket);

	stream.Write(&RequestCharacterNumber, sizeof(int32));

	PacketSession* pSession = GameServerManager::GetInst()->GetSession();
	pSession->Write(GP_GROUPACCEPT, stream.GetLength(), strPacket);

	SetVisibility(ESlateVisibility::Collapsed);
}

void UGroupRequestWidget::Cancel()
{
	SetVisibility(ESlateVisibility::Collapsed);
}