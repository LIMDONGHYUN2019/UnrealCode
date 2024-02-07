// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameInfo.h"
#include "NetSession.h"

/**
 * 
 */
class LECTUREGAME_API PacketSession :
	public NetSession
{
public:
	PacketSession();
	~PacketSession();

private:
	uint8 PacketBuffer[PACKET_LENGTH * 3];
	int32 iRemainLength;

public:
	bool ReadPacket(int32& iProtocol, uint8* pPacket, int32& iPacketLength);
};
