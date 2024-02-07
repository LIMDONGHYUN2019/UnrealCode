// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameInfo.h"

#define QueueSize 1000

class LECTUREGAME_API RecvQueue
{
public:
	RecvQueue();
	~RecvQueue();

private: // 여기서 큐구조체를 300개 미리확보한다. → 1000개로 확장
	FCriticalSection* m_pCrt;
	RecvQueueData	m_tQueue[QueueSize];
	int32	m_iHead;
	int32	m_iTail;
	int32	m_Count;

public:
	void SetCriticalSection(FCriticalSection* pCrt)
	{
		m_pCrt = pCrt;
	}

	int32 GetCount()	const
	{
		return m_Count;
	}

public:
	void push(int32 iProtocol, int32 iLength, uint8* packet);
	bool pop(int32& iProtocol, int32& iLength, uint8* packet);
};
