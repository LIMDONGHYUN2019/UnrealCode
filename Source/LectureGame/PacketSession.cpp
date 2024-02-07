// Fill out your copyright notice in the Description page of Project Settings.


#include "PacketSession.h"
#include "Crypt.h"

PacketSession::PacketSession()
{

	FMemory::Memset(PacketBuffer, 0, PACKET_LENGTH * 3);
	iRemainLength = 0;
}

PacketSession::~PacketSession()
{
}

bool PacketSession::ReadPacket(int32& iProtocol, uint8* pPacket, int32& iPacketLength)
{
	if (!NetSession::ReadPacket(iPacketLength, PacketBuffer + iRemainLength))
		return false;

	iRemainLength += iPacketLength;

	int32 iLength = 0;
	FMemory::Memcpy(&iLength, PacketBuffer, sizeof(int32));
	
	if (iLength > PACKET_LENGTH || iLength < 0)
	{
		iRemainLength = 0;
		return false;
	}

	if (iLength <= iRemainLength)
	{
		Crypt::DeCrypt((char*)PacketBuffer + sizeof(int32), (char*)PacketBuffer + sizeof(int32),
			iLength - sizeof(int32));

		int32 iPacketNumber = 0;
		int32 iProtocolNumber = 0;
		FMemory::Memcpy(&iPacketNumber, PacketBuffer + sizeof(int32), sizeof(int32));
		FMemory::Memcpy(&iProtocolNumber, PacketBuffer + sizeof(int32) * 2, sizeof(int32));
		iProtocol = iProtocolNumber;
		iPacketLength = iLength - sizeof(int32) * 3;
		FMemory::Memcpy(pPacket, PacketBuffer + sizeof(UINT) * 3, iPacketLength);
		if (iRemainLength - iLength > 0)
			FMemory::Memmove(PacketBuffer, PacketBuffer + iLength, iRemainLength - iLength);
		iRemainLength -= iLength;
		if (iRemainLength <= 0)
		{
			iRemainLength = 0;
			FMemory::Memset(PacketBuffer, 0, sizeof(PacketBuffer));
		}

		return true;
	}

	return false;
}
