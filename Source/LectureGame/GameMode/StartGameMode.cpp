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

	// Login Widget 정보를 읽어온다. 정보를 로딩하는것은 2가지가 있다.
	// Object, Class 2가지이다. Object는 실제 생성된 객체를 얻어오는 방식이고
	// Class는 이게 어떤 클래스 타입인지를 얻어오는 기능이다.
	// FObjectFinder는 애셋을 로딩해서 객체를 만들어내는 방식이고
	// FClassFinder는 애셋을 로딩해서 어떤 클래스 타입인지를 만들어내는 방식이다.
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
	// 게임서버에 접속한다.
	// 즉, 로그인 성공해서 정보를 가져온다.
	NetStream	stream;

	stream.SetBuffer(pPacket);

	// 유저번호를 넣어준다.
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