// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectWidget.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"
#include "../NetStream.h"
#include "../NetSession.h"
#include "../MainGameInstance.h"
#include "../GameServerManager.h"
#include "../PacketSession.h"
#include "../NetworkManager.h"
#include "../GameMode/SelectGameMode.h"
#include "SelectCameraManager.h"
#include "SelectCharacter.h"
#include "SelectPlayerController.h"

void USelectWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CreateCharacterButton = Cast<UButton>(GetWidgetFromName(TEXT("CreateCharacterButton")));
	GameStartButton = Cast<UButton>(GetWidgetFromName(TEXT("GameStartButton")));
	BackButton = Cast<UButton>(GetWidgetFromName(TEXT("BackButton")));
	DeleteButton = Cast<UButton>(GetWidgetFromName(TEXT("DeleteButton")));

	CreateCharacterButton->OnClicked.AddDynamic(this, &USelectWidget::CreateCharacter);
	GameStartButton->OnClicked.AddDynamic(this, &USelectWidget::GameStart);
	BackButton->OnClicked.AddDynamic(this, &USelectWidget::Back);
	DeleteButton->OnClicked.AddDynamic(this, &USelectWidget::Delete);
}

void USelectWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void USelectWidget::CreateCharacter()
{
	// 캐릭터 생성 버튼을 누르면 캐릭터 생성 외치로 카메라를 회전시키는 이벤트를 발생시킨다.
	ASelectGameMode* pGameMode = Cast<ASelectGameMode>(GetWorld()->GetAuthGameMode());

	if (pGameMode)
	{
		ASelectCameraManager* pCameraManager = pGameMode->GetCameraManager();

		if (pCameraManager)
		{
			pCameraManager->ChangeCreateCharacter();
		}
	}

	PacketSession* GameSession = GameServerManager::GetInst()->GetSession();
	GameSession->Write(GP_CREATE_CHARACTERINFO, 0, nullptr);
}

void USelectWidget::GameStart() // 게임시작 버튼을 누르면 적용
{
	ASelectGameMode* pGameMode = Cast<ASelectGameMode>(GetWorld()->GetAuthGameMode());

	if (pGameMode)
	{
	}

	ASelectPlayerController* pPlayerController = Cast<ASelectPlayerController>(GetWorld()->GetFirstPlayerController());

	if (pPlayerController)
	{
		ASelectCharacter* pSelectChar = pPlayerController->GetSelectCharacter();

		if (pSelectChar)
		{
			if (pSelectChar->GetSelectCharacterType() == ESelectCharType::Default)
			{
				
				NetworkManager::GetInst()->SetName(pSelectChar->GetServerCharacterName());
				FString myname = NetworkManager::GetInst()->GetName();
				LOG(TEXT("UserName : %s "), *(myname));
				// 서버로 선택한 캐릭터정보를 보내준다.
				int32	iCharacterNumber = pSelectChar->GetCharacterNumber();
				const char*	strJobName = pSelectChar->GetServerJob();

				NetStream	stream;

				uint8	strPacket[PACKET_LENGTH] = {};

				stream.SetBuffer(strPacket);

				stream.Write(&iCharacterNumber, 4);

				size_t	iLength = strlen(strJobName);

				// 직업 정보를 보내준다.
				stream.Write(&iLength, 4);
				stream.Write(strJobName, iLength);

				// 캐릭터 정보 요청 패킷을 보낸다.
				PacketSession* GameSession = GameServerManager::GetInst()->GetSession();

				GameSession->Write(GP_GAMESTART, stream.GetLength(), strPacket);
			}
		}
	}
}

void USelectWidget::Back()
{
	LOG(TEXT("Exit Button"));
}

void USelectWidget::Delete()
{
	LOG(TEXT("Delete Button"));
	ASelectPlayerController* pPlayerController = Cast<ASelectPlayerController>(GetWorld()->GetFirstPlayerController());

	if (pPlayerController)
	{
		ASelectCharacter* pSelectChar = pPlayerController->GetSelectCharacter();

		if (pSelectChar)
		{
			if (pSelectChar->GetCharacterNumber() > 0)
			{
				if (pSelectChar->GetSelectCharacterType() == ESelectCharType::Default)
				{
					int32	iCharacterNumber = pSelectChar->GetCharacterNumber();
					NetStream	stream;
					uint8	strPacket[PACKET_LENGTH] = {};
					stream.SetBuffer(strPacket);
					stream.Write(&iCharacterNumber, 4);

					PacketSession* GameSession = GameServerManager::GetInst()->GetSession();
					GameSession->Write(GP_DELETE_CHARACTERINFO, stream.GetLength(), strPacket);
				}
			}
		}
	}
}

