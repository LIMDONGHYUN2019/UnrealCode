// Fill out your copyright notice in the Description page of Project Settings.


#include "CreateCharacterWidget.h"
#include "../NetStream.h"
#include "../NetSession.h"
#include "../PacketSession.h"
#include "../MainGameInstance.h"
#include "../LoginServerManager.h"
#include "../GameServerManager.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"
#include "../GameMode/SelectGameMode.h"
#include "SelectCameraManager.h"
#include "SelectStatusWidget.h"
#include "SelectCharacter.h"
#include "SelectPlayerController.h"
//#include <Windows.h>

void UCreateCharacterWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CreateCharacterButton = Cast<UButton>(GetWidgetFromName(TEXT("CreateCharacterButton")));
	BackButton = Cast<UButton>(GetWidgetFromName(TEXT("BackButton")));
	StatusWidget = Cast<USelectStatusWidget>(GetWidgetFromName(TEXT("UI_SelectStatus")));
	InputName	= Cast<UEditableTextBox>(GetWidgetFromName(TEXT("InputName")));
	CreateFailMsgBox = Cast<UUserWidget>(GetWidgetFromName(TEXT("UI_CharacterCreateFailMsg")));
	CreateSuccessMsgBox = Cast<UUserWidget>(GetWidgetFromName(TEXT("UI_CharacterCreateSuccessMsg")));

	CreateCharacterButton->OnClicked.AddDynamic(this, &UCreateCharacterWidget::CreateCharacter);
	BackButton->OnClicked.AddDynamic(this, &UCreateCharacterWidget::Back);
}

void UCreateCharacterWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UCreateCharacterWidget::CreateCharacter()
{
	// 캐릭터 생성버튼을 누르면 서버로 캐릭터 생성 정보를 보낸다. 만약 생성할 수 있는 모든 캐릭터를 생성했다면 더이상 생성할 수 없다.
	// 이는 서버에서 판단하며 판단한 내용을 클라이언트로 반환해준다.
	// 어떤 캐릭터인지 캐릭터 정보를 패킷으로 만들어서 보낸다.
	// 모든 캐릭터 정보를 패킷으로 만드는 것이 아니라 캐릭터 번호? 정도만 만들어줘도 충분하다.
	ASelectPlayerController*	pController = Cast<ASelectPlayerController>(GetWorld()->GetFirstPlayerController());

	if (pController)        
	{
		ASelectCharacter*	pSelectCharacter = pController->GetSelectCharacter();

		if (pSelectCharacter)
		{
			const char* pJob = pSelectCharacter->GetServerJob();

			NetStream	stream;

			uint8	strPacket[PACKET_LENGTH] = {};

			stream.SetBuffer(strPacket);

			stream.Write(pJob, JOBNAME_SIZE);

			// 이름을 넣어준다.
			FString	strInputName = GetInputName();

			char	strName[USERNAME_SIZE]	= {};

			ConvertString(strName, strInputName);

			stream.Write(strName, USERNAME_SIZE);

			PacketSession* GameSession = GameServerManager::GetInst()->GetSession();

			GameSession->Write(GP_CREATECHARACTER, stream.GetLength(), strPacket);
		}
	}

	ResetInputName();
}

void UCreateCharacterWidget::Back()
{
	// 캐릭터 생성 버튼을 누르면 캐릭터 생성 외치로 카메라를 회전시키는 이벤트를 발생시킨다.
	ASelectGameMode* pGameMode = Cast<ASelectGameMode>(GetWorld()->GetAuthGameMode());

	if (pGameMode)
	{
		ASelectCameraManager* pCameraManager = pGameMode->GetCameraManager();

		if (pCameraManager)
		{
			// 카메라 전환을 하고 UI를 변경해준다.
			// 카메라 전환을 시작한다.
			pCameraManager->ChangeSelectCharacter();
		}
	}
}

void UCreateCharacterWidget::EnableInputName(bool bEnable)
{
	if (bEnable)
		InputName->SetVisibility(ESlateVisibility::Visible);

	else
		InputName->SetVisibility(ESlateVisibility::Collapsed);
}

FString UCreateCharacterWidget::GetInputName()
{
	return InputName->Text.ToString();
}

void UCreateCharacterWidget::ResetInputName()
{
	InputName->SetText(FText::FromString(TEXT("")));
}

void UCreateCharacterWidget::PopUpCreateFailMsgBox()
{
	CreateFailMsgBox->SetVisibility(ESlateVisibility::Visible);
}

void UCreateCharacterWidget::PopUpCreateSuccessMsgBox()
{
	CreateSuccessMsgBox->SetVisibility(ESlateVisibility::Visible);
}
