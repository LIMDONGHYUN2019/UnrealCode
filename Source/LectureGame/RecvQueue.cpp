// Fill out your copyright notice in the Description page of Project Settings.


#include "RecvQueue.h"

RecvQueue::RecvQueue()	:
	m_pCrt(nullptr),
	m_iHead(0),
	m_iTail(0),
	m_Count(0)
{
	//FScopeLock	lock(m_pCrt);
}

RecvQueue::~RecvQueue()
{
}

void RecvQueue::push(int32 iProtocol, int32 iLength, uint8* packet)
{
	FScopeLock	lock(m_pCrt);

	unsigned int iTail = (m_iTail + 1) % QueueSize;

	if (iTail == m_iHead)
		return;

	m_tQueue[iTail].iProtocol = iProtocol;
	m_tQueue[iTail].iLength = iLength;

	memset(m_tQueue[iTail].packet, 0, PACKET_LENGTH);
	memcpy(m_tQueue[iTail].packet, packet, iLength);

	m_iTail = iTail;

	++m_Count;
}

bool RecvQueue::pop(int32& iProtocol, int32& iLength, uint8* packet)
{
	FScopeLock	lock(m_pCrt);

	if (m_iHead == m_iTail)
		return false;

	int32 iHead = (m_iHead + 1) % QueueSize;

	iLength = m_tQueue[iHead].iLength;
	iProtocol = m_tQueue[iHead].iProtocol;

	memcpy(packet, m_tQueue[iHead].packet, m_tQueue[iHead].iLength);

	m_iHead = iHead;

	--m_Count;

	return true;
}
