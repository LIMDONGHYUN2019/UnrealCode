// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameInfo.h"

#define QueueSize 1000

class LECTUREGAME_API RecvQueue
{
public:
	RecvQueue();
	~RecvQueue();

private: // ���⼭ ť����ü�� 300�� �̸�Ȯ���Ѵ�. �� 1000���� Ȯ��
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
