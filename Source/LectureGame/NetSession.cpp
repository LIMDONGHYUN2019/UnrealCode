// Fill out your copyright notice in the Description page of Project Settings.


#include "NetSession.h"
#include "NetStream.h"
#include "Crypt.h"


NetSession::NetSession()
{	
	m_bConnect = false;
	m_pSocket = nullptr;
}

NetSession::~NetSession()
{
	Close();
}

bool NetSession::Connect(const FString& strIP, int32 iPort)
{
	m_pSocket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream,
		TEXT("default"), false);

	//m_pSocket->SetNonBlocking();

	FIPv4Address	ip;
	FIPv4Address::Parse(strIP, ip); // 설정

	TSharedRef<FInternetAddr>	addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	addr->SetIp(ip.Value); // 
	addr->SetPort(iPort);
	LOG(TEXT("Prev Connect"));
	// 레벨지정 x 

	m_bConnect = m_pSocket->Connect(*addr);

	return m_bConnect;
}

bool NetSession::Read(int32& iProtocol, int32& iLength, uint8* pPacket)
{
	bool bResult = m_pSocket->Recv(m_RecvPacket, 4096, iLength, ESocketReceiveFlags::None);

	if (!bResult || iLength <= 0)
		return false;

	// 암호화된 패킷을 복호화 해준다.
	/*Crypt::DeCrypt((char*)m_RecvPacket + sizeof(int32), (char*)m_RecvPacket + sizeof(int32),
		iLength - sizeof(int32));*/

	NetStream	stream; 
	stream.SetBuffer(m_RecvPacket);

	int32	iPacketLength = 0;
	stream.Read(&iPacketLength, sizeof(int32));

	int32	iPacketNumber = 0;
	stream.Read(&iPacketNumber, sizeof(int32));

	stream.Read(&iProtocol, sizeof(int32));

	stream.Read(pPacket, iPacketLength - sizeof(int32) * 3);

	iLength = iPacketLength - sizeof(int32) * 3;

	return bResult;
}

bool NetSession::ReadPacket(int32& iLength, uint8* pPacket)
{
	bool bResult = m_pSocket->Recv(m_RecvPacket, 4096, iLength, ESocketReceiveFlags::None);

	if (!bResult || iLength <= 0)
		return false;

	// 암호화된 패킷을 복호화 해준다.
	/*Crypt::DeCrypt((char*)m_RecvPacket + sizeof(int32), (char*)m_RecvPacket + sizeof(int32),
		iLength - sizeof(int32));*/

	FMemory::Memcpy(pPacket, m_RecvPacket, iLength);

	return bResult;
}

bool NetSession::Write(int32 iProtocol, int32 iLength, uint8* pPacket)
{
	// Packet을 만들어준다.
	NetStream	stream;
	stream.SetBuffer(m_SendPacket);

	// 패킷은 길이(4바이트) | 패킷번호(4바이트) | 프로토콜(4바이트) | 내용(4096)	
	int32	iPacketSize = iLength + sizeof(int32) * 3;

	stream.Write(&iPacketSize, sizeof(int32));
	int32	iPacketNumber = 1;
	stream.Write(&iPacketNumber, sizeof(int32));

	stream.Write(&iProtocol, sizeof(int32));

	if (iLength >= 0) // 원래 0보다 크게 설정이 되어있었나?
	{
		stream.Write(pPacket, iLength);

		// 암호화한다.
		Crypt::EnCrypt((char*)m_SendPacket + sizeof(int32), (char*)m_SendPacket + sizeof(int32),
			stream.GetLength() - sizeof(int32)); // int8 * 이므로
	}

	bool bResult = m_pSocket->Send(m_SendPacket, iPacketSize, iLength);

	return bResult;
}

void NetSession::Close()
{
	if (m_pSocket)
	{
		m_pSocket->Close();
		m_pSocket = nullptr;
	}
}
