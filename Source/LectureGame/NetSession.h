// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameInfo.h"

class LECTUREGAME_API NetSession
{
public:
	NetSession();
	~NetSession();

private:
	FSocket* m_pSocket;
	bool	m_bConnect;
	uint8	m_RecvPacket[PACKET_LENGTH];
	uint8	m_SendPacket[PACKET_LENGTH];

public:
	bool Connect(const FString& strIP, int32 iPort);
	bool Read(int32& iProtocol, int32& iLength, uint8* pPacket);
	bool ReadPacket(int32& iLength, uint8* pPacket);
	bool Write(int32 iProtocol, int32 iLength, uint8* pPacket);
	void Close();
};
 