
#include "ChattingServerManager.h"
#include "NetSession.h"
#include "PacketSession.h"
#include "RecvThread.h"
#include "NetStream.h"
#include "NetworkManager.h"



ChattingServerManager* ChattingServerManager::m_pInst = nullptr;


ChattingServerManager::ChattingServerManager()
{
	Thread = nullptr;
	RunThread = nullptr;
}

ChattingServerManager::~ChattingServerManager()
{
	Destroy();
}

bool ChattingServerManager::Init(const TCHAR* pIP, short sPort, int32 iUserNumber, const TCHAR* pID)
{
	ManagerBase::Init(pIP, sPort, iUserNumber, pID);

	m_pChattingSession = new PacketSession;

	bool bConnect = m_pChattingSession->Connect(pIP, sPort);

	if (bConnect)
	{
		// RecvThread를 만들어준다.
		RunThread = new RecvThread;
		RunThread->m_pManager = this;
		RunThread->m_pSession = m_pChattingSession;
		Thread = FRunnableThread::Create(RunThread, TEXT("ChattingRecvThread"));

		char UserName[32] = {};
		FMemory::Memcpy(UserName, NetworkManager::GetInst()->GetName(), 32);

		NetStream	stream;
		uint8	packet[PACKET_LENGTH] = {};
		stream.SetBuffer(packet);
		stream.Write(&iUserNumber, sizeof(int32));
		FString	strID = pID;
		//int32	iLength = strID.Len();
		//stream.Write(&iLength, sizeof(int32));
		stream.Write(UserName, sizeof(UserName));

		int32 iLength = stream.GetLength();

		m_pChattingSession->Write(CP_LOGIN, iLength, packet);
		return true;
	}

	return false;
}

void ChattingServerManager::Destroy()
{
	if (RunThread)
		RunThread->Exit();

	m_pChattingSession->Close();

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

	delete	m_pChattingSession;
}
