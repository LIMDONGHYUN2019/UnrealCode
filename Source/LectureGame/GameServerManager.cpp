// Fill out your copyright notice in the Description page of Project Settings.


#include "GameServerManager.h"
#include "NetSession.h"
#include "PacketSession.h"
#include "RecvThread.h"
#include "NetStream.h"

GameServerManager* GameServerManager::m_pInst = nullptr;

/*
로그인 관리자를 만들어준 이유는 NetworkManager에서 모든 패킷을 받게되면 패킷 프로토콜을 처리할때 로그인, 채팅, 인게임 패킷 들의 프로토콜이
순차적으로 1씩 증가하여 모두 다른 값으로 잡아주어야 하기 때문에 로그인관리자, 채팅관리자, 인게임 관리자 등을 만들어서 각 서버와 통신되는 기능별로
관리자 클래스를 만들고 스레드를 만들어서 해당 스레드에 들어오는 패킷들은 해당 관리자로 보내주어서 별도로 처리가 가능하도록 만들어주기 위해서이다.
*/

GameServerManager::GameServerManager()
{
	Thread = nullptr;
	RunThread = nullptr;
}

GameServerManager::~GameServerManager()
{
	Destroy();
}

bool GameServerManager::Init(const TCHAR* pIP, short sPort, int32 iUserNumber, const TCHAR* pID)
{
	ManagerBase::Init(pIP, sPort, iUserNumber, pID);

	m_pGameSession = new PacketSession;

	bool bConnect = m_pGameSession->Connect(pIP, sPort);

	if (bConnect)
	{
		// RecvThread를 만들어준다.
		RunThread = new RecvThread;

		RunThread->m_pManager = this;

		RunThread->m_pSession = m_pGameSession;

		Thread = FRunnableThread::Create(RunThread, TEXT("GameRecvThread"));

		// Connect 패킷을 보내준다.
		// 패킷을 만들어준다.
		NetStream	stream;

		uint8	packet[PACKET_LENGTH] = {};

		stream.SetBuffer(packet);
		// UserNumber, id
		stream.Write(&iUserNumber, sizeof(int32));

		FString	strID = pID;

		int32	iLength = strID.Len();

		stream.Write(&iLength, sizeof(int32));
		stream.Write(pID, iLength * sizeof(TCHAR));

		iLength = stream.GetLength();

		m_pGameSession->Write(GP_CONNECT, iLength, packet);

		return true;
	}

	return false;
}

void GameServerManager::Destroy()
{
	if (RunThread)
		RunThread->Exit();

	m_pGameSession->Close();

	if (Thread)
	{
		Thread->Kill();
		Thread->WaitForCompletion();
		Thread = nullptr;
	}

	if (RunThread)
	{
		delete RunThread;
		RunThread = nullptr;
	}

	delete	m_pGameSession;
}
