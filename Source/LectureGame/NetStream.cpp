// Fill out your copyright notice in the Description page of Project Settings.


#include "NetStream.h"

NetStream::NetStream()
{
	m_pBuffer = nullptr;
	m_iLength = 0;
}

NetStream::~NetStream()
{
}

void NetStream::Read(void* pData, int32 iSize)
{
	FMemory::Memcpy(pData, m_pBuffer + m_iLength, iSize);
	m_iLength += iSize;
}

void NetStream::Write(const void* pData, int32 iSize)
{
	FMemory::Memcpy(m_pBuffer + m_iLength, pData, iSize);
	m_iLength += iSize;
}
