// Fill out your copyright notice in the Description page of Project Settings.


#include "RecvThread.h"
#include "NetSession.h"
#include "RecvQueue.h"
#include "ManagerBase.h"
#include "PacketSession.h"
#include "NetStream.h"

RecvThread::RecvThread()
{
	bLoop = true;
}

RecvThread::~RecvThread()
{
	//delete	m_pMainGameSocket;
}

bool RecvThread::Init()
{
	return true;
}

// 스레드 실행 함수이다.
uint32 RecvThread::Run()
{
	while (bLoop)
	{
		uint8	Packet[PACKET_LENGTH] = {};
		int32	iProtocol = 0, iLength = 0;
		bool bResult = false;
		//bool bResult = m_pSession->Read(iProtocol, iLength, Packet);
		bResult = m_pSession->ReadPacket(iProtocol, Packet, iLength);
		if (!bResult)
			continue;

		// Alive Packet은 그냥 여기에서 바로 처리해버린다.
		if (iProtocol == GP_ALIVE)
		{
			uint8	packet[PACKET_LENGTH] = {};
			m_pSession->Write(GP_ALIVE, 0, packet);
		}

		else
		{
			RecvQueue* pQueue = m_pManager->GetRecvQueue();

			pQueue->push(iProtocol, iLength, Packet);
		}

		/*int32	iPacketLength = 0;
		FMemory::Memcpy(&iPacketLength, Packet, sizeof(int32));

		TCHAR	strText[256] = {};
		FMemory::Memcpy(strText, Packet + 4, iPacketLength);


		LOG(TEXT("Length : %d Text : %s"), iPacketLength, strText);

		int32	iNumber = 1002;

		FMemory::Memcpy(Packet, &iNumber, sizeof(int32));

		m_pMainGameSocket->Send(Packet, sizeof(int32), iRecvSize);

		LOG(TEXT("Send 100"), iPacketLength, strText);*/
	}

	return 0;
}

// 스레드를 stop 시켰을때 호출된다.
void RecvThread::Stop()
{
}

// 종료될때 호출되므로 스레드의 마무리 작업에 유용하다.
void RecvThread::Exit()
{
	bLoop = false;
}

