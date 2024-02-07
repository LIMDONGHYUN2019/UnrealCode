// Fill out your copyright notice in the Description page of Project Settings.
// 

#include "LoginServerManager.h"
#include "NetSession.h"
#include "PacketSession.h"
#include "RecvThread.h"

LoginManager* LoginManager::m_pInst = nullptr;

/*
�α��� �����ڸ� ������� ������ NetworkManager���� ��� ��Ŷ�� �ްԵǸ� ��Ŷ ���������� ó���Ҷ� �α���, ä��, �ΰ��� ��Ŷ ���� ����������
���������� 1�� �����Ͽ� ��� �ٸ� ������ ����־�� �ϱ� ������ �α��ΰ�����, ä�ð�����, �ΰ��� ������ ���� ���� �� ������ ��ŵǴ� ��ɺ���
������ Ŭ������ ����� �����带 ���� �ش� �����忡 ������ ��Ŷ���� �ش� �����ڷ� �����־ ������ ó���� �����ϵ��� ������ֱ� ���ؼ��̴�.
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
		// RecvThread�� ������ش�.
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
