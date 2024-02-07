// Fill out your copyright notice in the Description page of Project Settings.


#include "LoginWidget.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"
#include "../NetStream.h"
#include "../NetSession.h"
#include "../PacketSession.h"
#include "../MainGameInstance.h"
#include "../LoginServerManager.h"
#include "MemberShipWidget.h"

void ULoginWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 위젯들을 얻어온다.
	IDText = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("IDText")));
	PassText = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("PassText")));
	LoginButton = Cast<UButton>(GetWidgetFromName(TEXT("LoginButton")));
	MemberShipButton = Cast<UButton>(GetWidgetFromName(TEXT("MemberShipButton")));
	MemberShipWidget = Cast<UMemberShipWidget>(GetWidgetFromName(TEXT("UI_Membership")));

	// 버튼은 델리게이트를 가지고 있다. 여기에 함수를 등록시켜서 이 버튼이 눌러질때 호출될 함수를 등록하도록 한다.
	LoginButton->OnClicked.AddDynamic(this, &ULoginWidget::Login);
	MemberShipButton->OnClicked.AddDynamic(this, &ULoginWidget::Membership);
}

void ULoginWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void ULoginWidget::Login()
{
	//PrintViewport(2.f, FColor::Red, TEXT("Login"));
	// 로그인 패킷을 보낸다.
	// 패킷은 길이(4바이트) | 패킷번호(4바이트) | 프로토콜(4바이트) | 내용(4096)
	//uint8	packet[]
	NetStream	stream;

	// IDLength | ID | PassLength | Pass
	FString strID = IDText->Text.ToString();
	FString strPass = PassText->Text.ToString();

	uint8	strPacket[PACKET_LENGTH] = {};

	stream.SetBuffer(strPacket);

	int32	iIDLength = strID.Len() * sizeof(TCHAR);
	int32	iPassLength = strPass.Len() * sizeof(TCHAR);

	stream.Write(&iIDLength, sizeof(int32));
	stream.Write(*strID, iIDLength);

	stream.Write(&iPassLength, sizeof(int32));
	stream.Write(*strPass, iPassLength);

	PacketSession* LoginSession = LoginManager::GetInst()->GetSession();

	LoginSession->Write(LP_LOGIN, stream.GetLength(), strPacket);

	//PrintViewport(2.f, FColor::Red, FString::Printf(TEXT("ID : %s IDLength : %d size : %d Pass : %s"), *strID, iIDLength, sizeof(TCHAR), *strPass));
}

void ULoginWidget::Membership()
{
	//PrintViewport(2.f, FColor::Red, TEXT("Membership"));
	// 회원가입 창을 열어준다.
	switch (MemberShipWidget->GetVisibility())
	{
	case ESlateVisibility::Visible:
		MemberShipWidget->SetVisibility(ESlateVisibility::Collapsed);
		break;
	case ESlateVisibility::Collapsed:
		MemberShipWidget->SetVisibility(ESlateVisibility::Visible);
		break;
	}
}
