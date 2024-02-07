// Fill out your copyright notice in the Description page of Project Settings.


#include "GameServerManager.h"
#include "NetSession.h"
#include "PacketSession.h"
#include "RecvThread.h"
#include "NetStream.h"

GameServerManager* GameServerManager::m_pInst = nullptr;

/*
�α��� �����ڸ� ������� ������ NetworkManager���� ��� ��Ŷ�� �ްԵǸ� ��Ŷ ���������� ó���Ҷ� �α���, ä��, �ΰ��� ��Ŷ ���� ����������
���������� 1�� �����Ͽ� ��� �ٸ� ������ ����־�� �ϱ� ������ �α��ΰ�����, ä�ð�����, �ΰ��� ������ ���� ���� �� ������ ��ŵǴ� ��ɺ���
������ Ŭ������ ����� �����带 ���� �ش� �����忡 ������ ��Ŷ���� �ش� �����ڷ� �����־ ������ ó���� �����ϵ��� ������ֱ� ���ؼ��̴�.
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
		// RecvThread�� ������ش�.
		RunThread = new RecvThread;

		RunThread->m_pManager = this;

		RunThread->m_pSession = m_pGameSession;

		Thread = FRunnableThread::Create(RunThread, TEXT("GameRecvThread"));

		// Connect ��Ŷ�� �����ش�.
		// ��Ŷ�� ������ش�.
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
