// Fill out your copyright notice in the Description page of Project Settings.
// 

#include "LoginServerManager.h"
#include "NetSession.h"
#include "PacketSession.h"
#include "RecvThread.h"

LoginManager* LoginManager::m_pInst = nullptr;

/*
로그인 관리자를 만들어준 이유는 NetworkManager에서 모든 패킷을 받게되면 패킷 프로토콜을 처리할때 로그인, 채팅, 인게임 패킷 들의 프로토콜이
순차적으로 1씩 증가하여 모두 다른 값으로 잡아주어야 하기 때문에 로그인관리자, 채팅관리자, 인게임 관리자 등을 만들어서 각 서버와 통신되는 기능별로
관리자 클래스를 만들고 스레드를 만들어서 해당 스레드에 들어오는 패킷들은 해당 관리자로 보내주어서 별도로 처리가 가능하도록 만들어주기 위해서이다.
*/
LoginManager::LoginManager()
{
	Thread = nullptr;
	RunThread = nullptr;
	m_pLoginSession	= nullptr;
}

LoginManager::~LoginManager()
{
	Destroy();
}

bool LoginManager::Init()
{
	ManagerBase::Init();

	m_pLoginSession = new PacketSession;

	bool bConnect = m_pLoginSession->Connect(LOGINSERVER_IP, LOGIN_PORT);

	if (bConnect)
	{
		// RecvThread를 만들어준다.
		RunThread = new RecvThread;

		RunThread->m_pManager = this;

		RunThread->m_pSession = m_pLoginSession;

		Thread = FRunnableThread::Create(RunThread, TEXT("MainRecvThread"));

		return true;
	}

	delete	m_pLoginSession;
	m_pLoginSession	= nullptr;

	return false;
}

void LoginManager::Destroy()
{
	if (RunThread)
		RunThread->Exit();

	if (m_pLoginSession)
		m_pLoginSession->Close();

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

	if (m_pLoginSession)
	{
		delete	m_pLoginSession;
		m_pLoginSession = nullptr;
	}
}
