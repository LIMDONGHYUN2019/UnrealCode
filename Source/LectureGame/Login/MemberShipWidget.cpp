// Fill out your copyright notice in the Description page of Project Settings.


#include "MemberShipWidget.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"
#include "../NetStream.h"
#include "../NetSession.h"
#include "../PacketSession.h"
#include "../MainGameInstance.h"
#include "../LoginServerManager.h"

void UMemberShipWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 위젯들을 얻어온다.
	IDText = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("InputID")));
	PassText = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("InputPass")));
	NameText = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("InputName")));
	EMailText = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("InputEMail")));
	OkButton = Cast<UButton>(GetWidgetFromName(TEXT("ButtonOK")));
	CancelButton = Cast<UButton>(GetWidgetFromName(TEXT("ButtonCancel")));

	// 버튼은 델리게이트를 가지고 있다. 여기에 함수를 등록시켜서 이 버튼이 눌러질때 호출될 함수를 등록하도록 한다.
	OkButton->OnClicked.AddDynamic(this, &UMemberShipWidget::Join);
	CancelButton->OnClicked.AddDynamic(this, &UMemberShipWidget::Cancel);
}

void UMemberShipWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UMemberShipWidget::Join()
{
	NetStream	stream;
	FString strID = IDText->Text.ToString();
	FString strPass = PassText->Text.ToString();
	FString strName = NameText->Text.ToString();
	FString strEmail = EMailText->Text.ToString();

	uint8	strPacket[PACKET_LENGTH] = {};

	stream.SetBuffer(strPacket);

	int32	iIDLength = strID.Len() * sizeof(TCHAR);
	int32	iPassLength = strPass.Len() * sizeof(TCHAR);
	int32	istrNameLength = strName.Len() * sizeof(TCHAR);
	int32	istrEmailLength = strEmail.Len() * sizeof(TCHAR);

	stream.Write(&istrNameLength, sizeof(int32));
	stream.Write(*strName, istrNameLength);

	stream.Write(&istrEmailLength, sizeof(int32));
	stream.Write(*strEmail, istrEmailLength);

	stream.Write(&iIDLength, sizeof(int32));
	stream.Write(*strID, iIDLength);

	stream.Write(&iPassLength, sizeof(int32));
	stream.Write(*strPass, iPassLength);

	PacketSession* LoginSession = LoginManager::GetInst()->GetSession();

	LoginSession->Write(LP_MEMBERSHIP, stream.GetLength(), strPacket);

	PrintViewport(2.f, FColor::Red, FString::Printf(TEXT("MemberShip ID : %s IDLength : %d size : %d Pass : %s"), *strID, iIDLength, sizeof(TCHAR), *strPass));
}

void UMemberShipWidget::Cancel()
{
	SetVisibility(ESlateVisibility::Collapsed);
}


