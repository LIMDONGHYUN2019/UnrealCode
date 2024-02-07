// Fill out your copyright notice in the Description page of Project Settings.


#include "GroupWidget.h"
#include "GroupUserWidget.h"
#include "GroupUserData.h"
#include "../../Player/PlayerCharacter.h"
#include "../../NetStream.h"
#include "../../GameServerManager.h"
#include "../../PacketSession.h"

void UGroupWidget::NativeConstruct()
{
	Super::NativeConstruct();

	pLeave = Cast<UButton>(GetWidgetFromName(TEXT("Leave")));
	pCreate = Cast<UButton>(GetWidgetFromName(TEXT("Create")));
	pGroupList = Cast<UListView>(GetWidgetFromName(TEXT("GroupList")));
	pCloseButton = Cast<UButton>(GetWidgetFromName(TEXT("CloseButton")));

	pLeave->OnClicked.AddDynamic(this, &UGroupWidget::Leave);
	pCreate->OnClicked.AddDynamic(this, &UGroupWidget::Create);
	pCloseButton->OnClicked.AddDynamic(this, &UGroupWidget::CloseGroup);


	pAdminPlayer = nullptr;
}

void UGroupWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UGroupWidget::Create()
{
	UGroupUserData* pUser = NewObject<UGroupUserData>(this, UGroupUserData::StaticClass());

	APlayerCharacter* pPlayer = GetWorld()->GetFirstPlayerController()->GetPawn<APlayerCharacter>();
	FString Name = pPlayer->GetCharacterInfo().strName;
	FString Job = pPlayer->GetCharacterInfo().strJob;
	int32 iLevel = pPlayer->GetCharacterInfo().iLevel;

	pAdminPlayer = pPlayer;

	pUser->SetName(Name);
	pUser->SetJob(Job);
	pUser->SetLevel(iLevel);

	pGroupList->AddItem(pUser);
	
	pPlayer->SetIsGroup(true);
	pPlayer->SetIsGroupAdmin(true);

	pCreate->SetVisibility(ESlateVisibility::Collapsed);
	pLeave->SetVisibility(ESlateVisibility::Visible);

	NetStream stream;
	uint8 strPacket[PACKET_LENGTH] = {};
	stream.SetBuffer(strPacket);

	PacketSession* pSession = GameServerManager::GetInst()->GetSession();
	pSession->Write(GP_GROUPCREATE, stream.GetLength(), strPacket);
}           

void UGroupWidget::Leave()
{
	// ��Ƽ�� �������� �ٸ� ��Ƽ���� ����Ʈ�� �ϳ� �ٿ��ش�.
	// ���� �����̶�� ��Ƽ��ü�� ���ش�.

	if (pAdminPlayer->IsGroupAdmin())
	{
		// �׳� ���� ��Ƽ ���ִ� ���������� ������.
		for (int32 i = pGroupList->GetNumItems() - 1; i >= 0; --i)
		{
			UGroupUserData* pPlayer = Cast<UGroupUserData>(pGroupList->GetItemAt(i));

			PrintViewport(50.f, FColor::Blue, FString::Printf(TEXT("��Ƽ�ı��߽��ϴ�.  �����̸� : %s"), *pPlayer->GetName()));

			pGroupList->RemoveItem(pPlayer);

		}
		pAdminPlayer->SetIsGroup(false);
		pAdminPlayer->SetIsGroupAdmin(false);

		NetStream stream;
		uint8 strPacket[PACKET_LENGTH] = {};
		stream.SetBuffer(strPacket);

		PacketSession* pSession = GameServerManager::GetInst()->GetSession();
		pSession->Write(GP_GROUPLEAVE, stream.GetLength(), strPacket);
	}



	pCreate->SetVisibility(ESlateVisibility::Visible);
	pLeave->SetVisibility(ESlateVisibility::Collapsed);
}

void UGroupWidget::CloseGroup()
{
	SetVisibility(ESlateVisibility::Collapsed);
}

void UGroupWidget::AddPlayer(FString Name, FString Job, int32& Level)
{
	UGroupUserData* pUser = NewObject<UGroupUserData>(this, UGroupUserData::StaticClass());

	pUser->SetName(Name);
	pUser->SetJob(Job);
	pUser->SetLevel(Level);

	pGroupList->AddItem(pUser);

	if (GetVisibility() == ESlateVisibility::Collapsed)
		SetVisibility(ESlateVisibility::Visible);
}

void UGroupWidget::LeavePlayer()
{
	for (int32 i = pGroupList->GetNumItems() - 1; i >= 0; --i)
	{
		PrintViewport(50.f, FColor::Blue, FString::Printf(TEXT("��Ƽ�� ����")));

		UGroupUserData* pPlayer = Cast<UGroupUserData>(pGroupList->GetItemAt(i));

		pGroupList->RemoveItem(pPlayer);

	}

	pCreate->SetVisibility(ESlateVisibility::Visible);
	pLeave->SetVisibility(ESlateVisibility::Collapsed);
}