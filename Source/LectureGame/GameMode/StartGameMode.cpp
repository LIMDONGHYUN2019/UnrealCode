// Fill out your copyright notice in the Description page of Project Settings.


#include "StartGameMode.h"
#include "../MainGameInstance.h"
#include "../NetworkManager.h"
#include "../RecvQueue.h"
#include "../NetStream.h"
#include "../LoginServerManager.h"
#include "../GameServerManager.h"
#include "../Login/LoginWidget.h"

AStartGameMode::AStartGameMode()
{
	PrimaryActorTick.bCanEverTick = true;

	// Login Widget ������ �о�´�. ������ �ε��ϴ°��� 2������ �ִ�.
	// Object, Class 2�����̴�. Object�� ���� ������ ��ü�� ������ ����̰�
	// Class�� �̰� � Ŭ���� Ÿ�������� ������ ����̴�.
	// FObjectFinder�� �ּ��� �ε��ؼ� ��ü�� ������ ����̰�
	// FClassFinder�� �ּ��� �ε��ؼ� � Ŭ���� Ÿ�������� ������ ����̴�.
	static ConstructorHelpers::FClassFinder<UUserWidget>	LoginWidgetAsset(TEXT("WidgetBlueprint'/Game/UI/UI_Login.UI_Login_C'"));

	if (LoginWidgetAsset.Succeeded())
	{
		LoginWidgetClass = LoginWidgetAsset.Class;
	}

}

void AStartGameMode::StartPlay()
{
	Super::StartPlay();
}

void AStartGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (LoginWidgetClass)
	{
		LoginWidget = Cast<ULoginWidget>(CreateWidget(GetWorld(), LoginWidgetClass));

		if (LoginWidget)
		{
			LoginWidget->AddToViewport();
		}
	}

	if (!LoginManager::GetInst()->Init())
	{
		LoginManager::DestroyInst();
	}
}

void AStartGameMode::InitGameState()
{
	Super::InitGameState();
}

void AStartGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	//NetworkManager::DestroyInst();
	NetworkManager::GetInst()->LoginDisconnect();
}

void AStartGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RecvQueue* pQueue = LoginManager::GetInst()->GetRecvQueue();

	int32	iProtocol = 0, iLength = 0;
	uint8	packet[PACKET_LENGTH] = {};

	if (pQueue->pop(iProtocol, iLength, packet))
	{
		switch (iProtocol)
		{
		case LP_MEMBERSHIP:
			break;
		case LP_LOGIN:
			break;
		case LP_LOGOUT:
			break;
		case LP_LOGIN_SUCCESS:
			LoginSuccess(packet);
			break;
		case LP_LOGIN_FAIL:
			PrintViewport(2.f, FColor::Red, TEXT("Login Failed"));
			break;
		case LP_MEMBERSHIP_SUCCESS:
			PrintViewport(2.f, FColor::Red, TEXT("Membership Success"));
			break;
		case LP_MEMBERSHIP_FAIL:
			PrintViewport(2.f, FColor::Red, TEXT("Membership Failed"));
			break;
		}
	}
}

void AStartGameMode::LoginSuccess(uint8* pPacket)
{
	// ���Ӽ����� �����Ѵ�.
	// ��, �α��� �����ؼ� ������ �����´�.
	NetStream	stream;

	stream.SetBuffer(pPacket);

	// ������ȣ�� �־��ش�.
	int32	iUserNumber = -1;
	stream.Read(&iUserNumber, sizeof(int32));

	int32	iLength = 0, iIDLength = 0;
	TCHAR	strID[256] = {};
	stream.Read(&iIDLength, sizeof(int32));
	stream.Read(strID, iIDLength * sizeof(TCHAR));

	TCHAR	strIP[256] = {};
	stream.Read(&iLength, sizeof(int));
	stream.Read(strIP, iLength * sizeof(TCHAR));

	short	sPort = 0;
	stream.Read(&sPort, sizeof(short));

	NetworkManager::GetInst()->SetID(strID, iIDLength);
	NetworkManager::GetInst()->SetUserNumber(iUserNumber);

	GameServerManager::GetInst()->Init(strIP, sPort, iUserNumber, strID);
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("CharacterSelect"));
}