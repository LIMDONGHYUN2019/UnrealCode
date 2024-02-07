// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameInfo.h"

class LECTUREGAME_API NetStream
{
public:
	NetStream();
	~NetStream();

public:
	uint8* m_pBuffer;
	int32	m_iLength;

public:
	int32 GetLength()	const
	{
		return m_iLength;
	}

	uint8* GetPacket()	const
	{
		return m_pBuffer;
	}

public:
	void SetBuffer(uint8* pBuffer)
	{
		m_pBuffer = pBuffer;
	}

public:
	void Read(void* pData, int32 iSize);
	void Write(const void* pData, int32 iSize);
};
